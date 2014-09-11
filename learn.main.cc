//   -*- c++ -*-

#include "learn.h"


#include <iostream>
#include <fstream>

#include "redo_parse.h"
#include "tp_eigenwords.h"
#include "maximum_likelihood.h"
#include "contrast.h"
#include "tokenize.h"




// It should be random for production code. 
// #define REPRODUCIBLE
#define TRULY_RANDOM
#include "utilities/z.Template.h"
#include "utilities/iostream_box.h"

int
main(int argc,char** argv)
{
  Eigen::initParallel();  // this will help eigen not walk on top of itself.

  //////////////////////////////////////////////////////////////////////////////////
  //
  //  Read the command line parameters and set up basic variables and basic files
  //
  //////////////////////////////////////////////////////////////////////////////////
  
  std::string sentence_file, eigen_file, latex_prefix;
  int gram_number, repeats_per_level;
  double update_rate,scaling, noise;
  bool use_eager;
  std::string comment;
  boost::tie(repeats_per_level,latex_prefix, eigen_file, gram_number, sentence_file, update_rate, scaling, noise, comment,use_eager)
    = auto_parse::parse_argv(argc, argv);
  time_t start_time = time(0);  // used for final timing
  time_t running_time = time(0);  // used for timing 
  time_t last_print_time = time(0);  // used to print about once a minute
  std::ofstream latex(latex_prefix + ".tex");
  auto_parse::latex_header(latex);  // write a "...\begin{document}" on latex_file

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    Read in the Eigendictionary
  //
  // It is used in both LR parser and likelihood.
  //
  //////////////////////////////////////////////////////////////////////////////////

    std::ifstream in(eigen_file);
    auto_parse::Eigenwords dictionary(in,gram_number); 
    auto_parse::Eigenwords parent_dictionary(dictionary); 
    auto_parse::Eigenwords child_dictionary = dictionary.with_constant_row_sum_squares(); 
    int dim = dictionary.dimension();

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    SET UP THE CRUDE TOKENIZER
  //
  // Note: if the corpus is too big to fit in memory, some other method should be used
  // Doing it this way allows a way of easilly using OpenMP.
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::Tokenize corpus(sentence_file);
  std::vector<auto_parse::Words> corpus_in_memory;
  corpus.reset();
  while(!corpus.eof())
    corpus_in_memory.push_back(corpus.next_sentence());

  //////////////////////////////////////////////////////////////////////////////////
  //
  //   Print a big friendly blurb about our processing
  //
  //////////////////////////////////////////////////////////////////////////////////
  
  std::cout << "\n\n               " << comment  << "\n\n\n" << std::endl;
  std::cout << "     --corpus = " << sentence_file << std::endl;
  std::cout << " --dictionary = " << eigen_file << std::endl;
  std::cout << "--gram_number = " << gram_number << std::endl;
  std::cout << "      --latex = " << latex_prefix << std::endl;
  std::cout << "--update_rate = " << update_rate << "    (" << 1 - update_rate << " * old_model + " << update_rate << " * new_model)" << std::endl;
  std::cout << "    --scaling = " << scaling << "    (-(Y - Yhat)^2 + " << scaling << " * log(probability skip) )" << std::endl;
  std::cout << "      --eager = " << use_eager << std::endl;
  std::cout << "      --noise = " << noise << "    randomly pick best action with about accuracy noise level." << std::endl;
  std::cout << "repeats_per_level= " << repeats_per_level << std::endl;
  std::cout << "   --comment = " << comment << std::endl;
  std::cout << "Read a dictionary of size: " << dictionary.size()<< " x " << dim
	    << " (time " << time(0) - running_time << " sec)" << std::endl;      running_time = time(0);
  std::cout << "Training on " << corpus_in_memory.size() << " sentence.    " << std::endl;
  std::cout << "number threads = " << auto_parse::number_of_threads_used() << "." << std::endl;
      

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                            LR Parser setup
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::Feature_generator* p_feature_generator = 0;
  if(use_eager)
    {
      auto_parse::set_all_actions(auto_parse::eager_actions);  // programming by side effects.
      p_feature_generator = new auto_parse::Feature_generator(eager_features(dictionary));
    }
  else
    {
      auto_parse::set_all_actions(auto_parse::standard_actions);
      p_feature_generator = new auto_parse::Feature_generator(standard_features(dictionary));
    };
  auto_parse::Model old_model(auto_parse::generate_linear_model(p_feature_generator->dimension()));
  auto_parse::Statistical_parse parser(old_model,*p_feature_generator,noise);

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                     Likelihood setup (mostly transition_probability setup)
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::TP_eigenwords tp_left(dictionary,dictionary,scaling);  
  auto_parse::TP_eigenwords tp_right(dictionary,dictionary,scaling);  
  auto_parse::TP_eigenwords tp_root(auto_parse::Eigenwords::create_root_dictionary(),dictionary,scaling);  
  auto_parse::Likelihood likelihood(tp_left,tp_right,tp_root);

  //////////////////////////////////////////////////////////////////////////////////
  //
  //           Speed hack (aka SGD). Don't process all the data each update.
  //           Instead, process more and more as time goes on.
  //
  // Timing: it takes about 10 seconds to futz with the matrixes in an update.
  // So there is no point in spending only 2 seconds processing data.  At around
  // 40K, it now takes about 10 seconds for the data and 10 seconds for futzing.
  // so that is a resonable minimumal amount to play with.
  //
  // (I originally had 10K, 20K, 50K, 100K.)
  //
  //////////////////////////////////////////////////////////////////////////////////

  int n = corpus_in_memory.size();  // (About 462k)
  assert(n > 400*1000);
  std::vector<int> number_to_train_on(6*repeats_per_level, n);  // default = n
  int K = 1000;
  for(int i = 0; i < repeats_per_level;++i)
    number_to_train_on[i] = 40*K;
  for(int i = 1 * repeats_per_level; i < 2 * repeats_per_level;++i)
    number_to_train_on[i] = 50*K;
  for(int i = 2 * repeats_per_level; i < 3 * repeats_per_level;++i)
    number_to_train_on[i] = 80*K;
  for(int i = 3 * repeats_per_level; i < 4 * repeats_per_level;++i)
    number_to_train_on[i] = 100*K;
  for(int i = 4 * repeats_per_level; i < 5 * repeats_per_level;++i)
    number_to_train_on[i] = 200*K;
      
  //////////////////////////////////////////////////////////////////////////////////
  //
  //                            MAIN LOOP
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::vector<auto_parse::Words>::const_iterator end_read_interval = corpus_in_memory.end();

  for(unsigned int rounds = 0; rounds < number_to_train_on.size(); ++rounds)
    {
      std::vector<auto_parse::Words>::const_iterator begin = end_read_interval;
      if(begin + number_to_train_on[rounds] > corpus_in_memory.end())
	begin = corpus_in_memory.begin();
      std::vector<auto_parse::Words>::const_iterator end = begin + number_to_train_on[rounds];
      end_read_interval = end;
	
      double sampling_rate = .05;  // this generates about a factor of 10 speed up by only computing X'X 5% of the time


      //
      //  Set up an ostream that will keep a running tab of where we are in the output
      //
      std::stringstream s;
      s << number_to_train_on[rounds]/1000 << "k: " << rounds << " | ";
      std::string ostrm_prefix = s.str();
      boost::iostreams::filtering_ostream ostrm;
      ostrm.push(box_output_filter(100,ostrm_prefix , "" , "" ));
      ostrm.push(std::cout);
	
      ///////////////////////////////////////////////
      //                                           //
      //           Likelihood --> Model            //
      //                                           //
      ///////////////////////////////////////////////

      auto_parse::Model model = likelihood_to_model(likelihood, parser,
						    *p_feature_generator,
						    sampling_rate,
						    begin, end,
						    ostrm);
      old_model.tweak(model, update_rate);
      parser.new_model(old_model);
      ostrm << "\t\t\t\tTraining time " << time(0) - running_time << " sec." << std::endl;      running_time = time(0);

      ///////////////////////////////////////////////
      //                                           //
      //  Model  --> Parsed corpus -->  MLE        //
      //                                           //
      ///////////////////////////////////////////////

      likelihood = model_to_likelihood(parent_dictionary, child_dictionary,
				       parser, scaling, begin, end);
      ostrm << "\t\t\t\tMLE time " << time(0) - running_time << " sec." << std::endl;      running_time = time(0);

      ///////////////////////////////////////////////
      //                                           //
      //               EVALUATION                  //
      //                                           //
      ///////////////////////////////////////////////

      if(time(0) - last_print_time > 600)  // print once every 10 minutes
	{
	  last_print_time = time(0);
	  std::vector<int> which_sentences {3643, 2, 4, 10, 17, 26};
	  ostrm <<   evaluation(rounds, ostrm, latex, dictionary, parser, likelihood, which_sentences, begin, end) << std::endl;
	  ostrm << "Evaluation time " << time(0) - running_time << " sec." << std::endl;      running_time = time(0);
	}
    }
  auto_parse::latex_footer(latex);

  ///////////////////////////////////////////////////////////////////
  //
  //  Save the model.  (The likelihood can be recreated, so don't
  //  bother saving that.)
  //
  ///////////////////////////////////////////////////////////////////
    
  /*  Insert code here */

    
  ///////////////////////////////////////////////////////////////////
  //
  //  Print some nice output.
  //
  //  (This should be moved to a different file which reads in the
  //  model and then generates this output.  But we can't save yet,
  //  so this is a stop gap solution.)
  //
  ///////////////////////////////////////////////////////////////////

  int num_seconds = time(0) - start_time;
  int num_minutes = num_seconds / 60;
  int num_hours = num_minutes / 60;
  int extra_minutes = num_minutes - 60 * num_hours;

  std::ofstream latex_final(latex_prefix + ".final.tex");  // open it here in case the program gets killed.
  auto_parse::latex_header(latex_final);
  latex_final << "\\section*{" << comment << "}\n";
  latex_final << "Parameters:\n\\begin{itemize}";
  latex_final << "\\item corpus: \\verb\"" << sentence_file << "\"" << std::endl;
  latex_final << "\\item latex: \\verb\"" << latex_prefix << "\"" << std::endl;
  latex_final << "\\item update: " << update_rate << std::endl;
  latex_final << "\\item scaling: " << scaling << std::endl;
  latex_final << "\\item noise: " << noise << std::endl;
  latex_final << "\\item eager: " << use_eager << std::endl;
  latex_final << "\\item repeats: " << repeats_per_level << std::endl;
  latex_final << "\\item Trained on " << corpus_in_memory.size() << " sentence.    " << std::endl;
  latex_final << "\\item Threads used: " << auto_parse::number_of_threads_used()  << std::endl;
  latex_final << "\\item Total time: " << num_seconds << "    (" << num_hours << ":" << extra_minutes  << " h:m)" << std::endl;
  latex_final  << "\\item Date: \\today \n" ;
  latex_final << "\\end{itemize}\\newpage" << std::endl;

  std::vector<int> which_sentences {3643, 2, 4, 10, 17, 26};
  evaluation(*(number_to_train_on.end()-1), std::cout, latex_final, dictionary, parser, likelihood, which_sentences,
	     begin(corpus_in_memory), end(corpus_in_memory));

    which_sentences = std::vector<int>();
    std::multimap<int,std::vector<auto_parse::Words>::const_iterator> size_to_index;
    for(std::vector<auto_parse::Words>::const_iterator i =  begin(corpus_in_memory); i != end(corpus_in_memory); ++i)
      size_to_index.insert(std::make_pair(i->size(), i));

    latex_final << "\n\\newpage\n\n";

    for(int sentence_size = 0; sentence_size != 25;++sentence_size)
      {
	if(sentence_size == 18)
	  latex_final << "\\scriptsize\n";
	if(size_to_index.count(sentence_size) >= 3)
	  {
	    auto i = size_to_index.lower_bound(sentence_size);
	    latex_final << "\\subsection*{" << sentence_size -1 << " words  : \\#" << i->second - begin(corpus_in_memory) << " + " << std::endl;
	    auto s1 = *(i->second);
	    auto p1 = redo_parse(s1, parser(s1)).parse();
	    ++i;
	    latex_final << "\\#"  << i->second - begin(corpus_in_memory) << "}  " << std::endl;
	    auto s2 = *(i->second);
	    auto p2 = redo_parse(s2, parser(s2)).parse();
	    likelihood.decorate(p1, dictionary).latex(latex_final);
	    likelihood.decorate(p2, dictionary).latex(latex_final);
	  }
      }
    auto_parse::latex_footer(latex_final);

    std::cout << "\n\n               FINISHED: " << comment  << "\n\n\n" << std::endl;
}

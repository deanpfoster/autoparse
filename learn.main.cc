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
  //  Eigen::initParallel();  // this will help eigen not walk on top of itself.

  //////////////////////////////////////////////////////////////////////////////////
  //
  //  Read the command line parameters and set up basic variables and basic files
  //
  //////////////////////////////////////////////////////////////////////////////////
  
  std::string sentence_file, eigen_file, latex_prefix;
  int gram_number, repeats_per_level;
  double update_rate,scaling, noise;
  bool use_eager, r2l;
  std::string comment;
  auto_parse::File_names files;
  boost::tie(files, repeats_per_level, update_rate, scaling, noise, comment,use_eager, r2l) = auto_parse::parse_argv(argc, argv);
  boost::tie(sentence_file, eigen_file, gram_number, latex_prefix) = files;
  time_t start_time = time(0);  // used for final timing
  time_t last_print_time = time(0);  // used to print about once a minute
  std::ofstream latex(latex_prefix + ".tex");
  auto_parse::latex_header(latex);  // write a "...\begin{document}" on latex_file
  using auto_parse::print_time;
  print_time("starting the clock");

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

    auto_parse::Tokenize corpus_file(sentence_file,&dictionary.lexicon());
  std::vector<auto_parse::Words> raw_corpus;
  while(!corpus_file.eof())
    raw_corpus.push_back(corpus_file.next_sentence());
  std::vector<auto_parse::Words>* p_corpus = &raw_corpus;
  if(r2l)
    p_corpus = new std::vector<auto_parse::Words>(auto_parse::reverse(raw_corpus)); // reverses each sentence

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
  std::cout << "    --reverse = " << r2l   << " process R to L" << std::endl;
  std::cout << "repeats_per_level= " << repeats_per_level << std::endl;
  std::cout << "   --comment = " << comment << std::endl;
  std::cout << "Read a dictionary of size: " << dictionary.size()<< " x " << dim << print_time("reading") << std::endl;
  std::cout << "Training on " << p_corpus->size() << " sentence.    " << std::endl;
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
  auto_parse::TP_eigenwords tp_root(auto_parse::Eigenwords::create_root_dictionary(dictionary.lexicon()),dictionary,scaling);  
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

  int n = p_corpus->size();  // (About 462k)
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

  std::vector<auto_parse::Words>::const_iterator end_read_interval = p_corpus->end();

  for(unsigned int rounds = 0; rounds < number_to_train_on.size(); ++rounds)
    {
      //
      // Compute which sentences we should be using
      //
      std::vector<auto_parse::Words>::const_iterator begin = end_read_interval;
      if(begin + number_to_train_on[rounds] > p_corpus->end())
	begin = p_corpus->begin();
      std::vector<auto_parse::Words>::const_iterator end = begin + number_to_train_on[rounds];
      end_read_interval = end;
	
      // this generates about a factor of 10 speed up by only computing X'X 5% of the time
      double sampling_rate = .05;

      ////////////////////////////////////////////////////
      //                                               //
      //  Set up an ostream that will keep a          //
      //  running tab of where we are in the output  //
      //                                            //
      ///////////////////////////////////////////////

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
      ostrm << print_time("Training");

      ///////////////////////////////////////////////
      //                                           //
      //  Model  --> Parsed corpus -->  MLE        //
      //                                           //
      ///////////////////////////////////////////////

      likelihood = model_to_likelihood(parent_dictionary, child_dictionary,
				       parser, scaling, begin, end);
      ostrm << print_time("MLE");

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
	  ostrm << print_time("Evaluation");
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
  auto_parse::latex_header(latex_final,latex_prefix);
  latex_final << "\\section*{" << comment << "}\n";
  latex_final << "Parameters:\n\\begin{itemize}";
  latex_final << "\\item corpus: \\verb\"" << sentence_file << "\"" << std::endl;
  latex_final << "\\item latex: \\verb\"" << latex_prefix << "\"" << std::endl;
  latex_final << "\\item update: " << update_rate << std::endl;
  latex_final << "\\item scaling: " << scaling << std::endl;
  latex_final << "\\item noise: " << noise << std::endl;
  latex_final << "\\item eager: " << use_eager << std::endl;
  latex_final << "\\item reverse: " << r2l << std::endl;
  latex_final << "\\item repeats: " << repeats_per_level << std::endl;
  latex_final << "\\item Trained on " << p_corpus->size() << " sentence.    " << std::endl;
  latex_final << "\\item Threads used: " << auto_parse::number_of_threads_used()  << std::endl;
  latex_final << "\\item Total time: " << num_seconds << "    (" << num_hours << ":" << extra_minutes  << " h:m)" << std::endl;
  latex_final  << "\\item Date: \\today \n" ;
  latex_final << "\\end{itemize}\\newpage" << std::endl;

  std::vector<int> which_sentences {3643, 2, 4, 10, 17, 26};
  evaluation(*(number_to_train_on.end()-1), std::cout, latex_final, dictionary, parser, likelihood, which_sentences,
	     begin(*p_corpus), end(*p_corpus));

    which_sentences = std::vector<int>();
    std::multimap<int,std::vector<auto_parse::Words>::const_iterator> size_to_index;
    for(std::vector<auto_parse::Words>::const_iterator i =  begin(*p_corpus); i != end(*p_corpus); ++i)
      size_to_index.insert(std::make_pair(i->size(), i));

    latex_final << "\n\\newpage\n\n";

    for(int sentence_size = 0; sentence_size != 25;++sentence_size)
      {
	if(sentence_size == 18)
	  latex_final << "\\scriptsize\n";
	if(size_to_index.count(sentence_size) >= 5)
	  {
	    auto i = size_to_index.lower_bound(sentence_size);
	    latex_final << "\\newpage\n\\subsection*{" << sentence_size -1 << " words  : ";
	    latex_final << "\\#" << i->second - begin(*p_corpus);
	    auto s1 = *(i->second);
	    auto p1 = redo_parse(s1, parser.best_parse(s1)).parse();
	    ++i;
	    latex_final << " + \\#"  << i->second - begin(*p_corpus);
	    auto s2 = *(i->second);
	    auto p2 = redo_parse(s2, parser.best_parse(s2)).parse();
	    ++i;
	    latex_final << " + \\#"  << i->second - begin(*p_corpus);
	    auto s3 = *(i->second);
	    auto p3 = redo_parse(s3, parser.best_parse(s3)).parse();
	    ++i;
	    latex_final << " + \\#"  << i->second - begin(*p_corpus);
	    auto s4 = *(i->second);
	    auto p4 = redo_parse(s4, parser.best_parse(s4)).parse();
	    latex_final << "}  " << std::endl;
	    if(r2l)
	      {
		likelihood.decorate(p1, dictionary).latex_reversed(latex_final);
		likelihood.decorate(p2, dictionary).latex_reversed(latex_final);
		likelihood.decorate(p3, dictionary).latex_reversed(latex_final);
		likelihood.decorate(p4, dictionary).latex_reversed(latex_final);
	      }
	    else
	      {
		likelihood.decorate(p1, dictionary).latex(latex_final);
		likelihood.decorate(p2, dictionary).latex(latex_final);
		likelihood.decorate(p3, dictionary).latex(latex_final);
		likelihood.decorate(p4, dictionary).latex(latex_final);
	      }
	  }
      }
    auto_parse::latex_footer(latex_final);

    std::cout << "\n\n               FINISHED: " << comment  << "\n\n\n" << std::endl;
}

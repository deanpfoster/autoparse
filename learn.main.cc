//   -*- c++ -*-

#include "learn.h"

#include "tp_eigenwords.h"
#include "tp_iid.h"
#include "maximum_likelihood.h"
#include "tokenize.h"
#include "utilities/iostream_box.h"

// It should be random for production code. 
// #define REPRODUCIBLE
#define TRULY_RANDOM
#include "utilities/z.Template.h"

int
main(int argc,char** argv)
{
  //  Eigen::initParallel();  // this will help eigen not walk on top of itself.

  //////////////////////////////////////////////////////////////////////////////////
  //
  //  Read the command line parameters and set up basic variables and basic files
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::Parse_args a(argc, argv);

  time_t last_print_time = time(0);  // used to print about once a minute
  std::ofstream latex(a.latex_prefix + ".tex");
  auto_parse::latex_header(latex);  // write a "...\begin{document}" on latex_file
  using auto_parse::print_time;
  print_time("starting the clock");

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    Read in the Eigendictionary
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::ifstream in(a.eigen_file);
  auto_parse::Eigenwords dictionary(in,a.gram_number); 
  auto_parse::Eigenwords parent_dictionary(dictionary); 
  auto_parse::Eigenwords child_dictionary = dictionary.with_constant_row_sum_squares(); 

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    SET UP THE CRUDE TOKENIZER
  //
  //  Note: if the corpus is too big to fit in memory, some other method should be used
  //  Doing it this way allows a way of easilly using OpenMP.
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::Tokenize corpus_file(a.sentence_file,&dictionary.lexicon());
  std::vector<auto_parse::Words> raw_corpus;
  while(!corpus_file.eof())
    raw_corpus.push_back(corpus_file.next_sentence());
  std::vector<auto_parse::Words>* p_corpus = &raw_corpus;
  if(a.r2l)
    p_corpus = new std::vector<auto_parse::Words>(auto_parse::reverse(raw_corpus)); // reverses each sentence

  //////////////////////////////////////////////////////////////////////////////////
  //
  //   Print a big friendly blurb about our processing
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::cout << a.friendly_message(dictionary, *p_corpus);

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                            LR Parser setup
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::Feature_generator* p_feature_generator = 0;
  auto_parse::Model old_model;
  if(a.use_eager)
    {
      old_model = auto_parse::generate_linear_model(p_feature_generator->dimension(), auto_parse::eager_actions);
      p_feature_generator = new auto_parse::Feature_generator(eager_features(dictionary));
    }
  else
    {
      old_model = auto_parse::generate_linear_model(p_feature_generator->dimension(), auto_parse::standard_actions);
      p_feature_generator = new auto_parse::Feature_generator(standard_features(dictionary));
    };
  auto_parse::Statistical_parse parser(old_model,*p_feature_generator,a.noise);

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                     Likelihood setup
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::TP_eigenwords tp_left(dictionary,dictionary,a.scaling);  
  auto_parse::TP_eigenwords tp_right(dictionary,dictionary,a.scaling);  
  auto_parse::TP_iid        tp_root(dictionary.lexicon().size(),a.scaling);  
  auto_parse::Likelihood    likelihood(tp_left,tp_right,tp_root);

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
  //////////////////////////////////////////////////////////////////////////////////

  int K = 1000;
  int n = p_corpus->size();  // (default corpus has 462k)
  assert(n > 100*K);
  std::vector<int> number_to_train_on;
  for(int i = 0; i < a.repeats_per_level;++i)
    number_to_train_on.push_back(40*K);
  for(int i = 0; i < a.repeats_per_level;++i)
    number_to_train_on.push_back(60*K);
  for(int i = 0; i < a.repeats_per_level;++i)
    number_to_train_on.push_back(80*K);
  if(n > 100*K)
    for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(100*K);
  if(n > 200*K)
    for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(200*K);
  if(n > 400*K)
    for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(400*K);
  if(n > 1000*K)
    for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(1000*K);
  for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(n);
      
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
      old_model.tweak(model, a.update_rate);
      parser.new_model(old_model);
      ostrm << print_time("Training");

      ///////////////////////////////////////////////
      //                                           //
      //  Model  --> Parsed corpus -->  MLE        //
      //                                           //
      ///////////////////////////////////////////////

      likelihood = model_to_likelihood(parent_dictionary, child_dictionary,
				       parser, a.scaling, begin, end);
      ostrm << print_time("MLE");

      ///////////////////////////////////////////////
      //                                           //
      //               EVALUATION                  //
      //                                           //
      ///////////////////////////////////////////////

      if(time(0) - last_print_time > 600)  // Evaluate once every 10 minutes
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
  //  Saving the model.  
  //
  ///////////////////////////////////////////////////////////////////
    
  /*  Insert code here */
    
  ///////////////////////////////////////////////////////////////////
  //
  //  Print some nice latex noutput.
  //
  ///////////////////////////////////////////////////////////////////

  a.print_latex(*p_corpus, likelihood, number_to_train_on, dictionary, parser);

  std::cout << "\n\n               FINISHED: " << a.comment  << "\n\n\n" << std::endl;
}

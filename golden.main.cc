//   -*- c++ -*-

#include "learn.h"

#include "tp_eigenwords.h"
#include "tp_iid.h"
#include "maximum_likelihood.h"
#include "conll_2006.h"
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
  //                    READ IN THE GOLD STANDARD PARSES
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::ifstream  conll(a.golden_file);
  std::vector<auto_parse::Words> raw_corpus;
  std::vector<auto_parse::Gold_standard> gold_parses;
  while(!conll.eof())
    {
      auto_parse::Gold_standard d = auto_parse::read_conll(conll, dictionary.lexicon());
      gold_parses.push_back(d);
      raw_corpus.push_back(d.standard().sentence());
    };
  std::vector<auto_parse::Words>* p_corpus = &raw_corpus;
  if(a.r2l)
    {
      p_corpus = new std::vector<auto_parse::Words>(auto_parse::reverse(raw_corpus)); // reverses each sentence
      assert(0); // need to write code to reverse the parses
    };

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
      p_feature_generator = new auto_parse::Feature_generator(eager_features(dictionary));
      old_model = auto_parse::generate_linear_model(p_feature_generator->dimension(), auto_parse::eager_actions);
    }
  else
    {
      p_feature_generator = new auto_parse::Feature_generator(standard_features(dictionary));
      old_model = auto_parse::generate_linear_model(p_feature_generator->dimension(), auto_parse::standard_actions);
    };
  auto_parse::Statistical_parse parser(old_model,*p_feature_generator,a.noise);

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
  std::vector<int> number_to_train_on;
  if(n > 40*K)
    for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(40*K);
  if(n > 60*K)
    for(int i = 0; i < a.repeats_per_level;++i)
      number_to_train_on.push_back(60*K);
  if(n > 80*K)
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

  std::cout << "Gold standard for 3643:" << std::endl;
  std::cout << gold_parses[3643].standard() << std::endl;


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
      std::vector<auto_parse::Gold_standard>::const_iterator  gold_begin = gold_parses.begin() + (begin - p_corpus->begin());
	
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
      //        gold standard --> Model            //
      //                                           //
      ///////////////////////////////////////////////

      auto_parse::Model model = gold_standard_to_model(parser,
						       *p_feature_generator,
						       sampling_rate,
						       gold_begin, 
						       begin, end);
      old_model.tweak(model, a.update_rate);
      parser.new_model(old_model);
      ostrm << print_time("Training");

      ///////////////////////////////////////////////
      //                                           //
      //               EVALUATION                  //
      //                                           //
      ///////////////////////////////////////////////

      //      if(time(0) - last_print_time > 600)  // Evaluate once every 10 minutes
      if(time(0) - last_print_time > 30) 
	{
	  auto_parse::Likelihood likelihood = model_to_likelihood(parent_dictionary, child_dictionary,
								  parser, a.scaling, begin, end);


	  last_print_time = time(0);
	  std::vector<int> which_sentences {3643, 2, 4, 10, 17, 26};
	  ostrm <<   evaluation(rounds, ostrm, latex, dictionary, parser, likelihood, which_sentences, begin, end) << std::endl;
	  ostrm <<   golden_evaluation(parser, begin, end, gold_begin) << std::endl;
	  
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

  auto_parse::Likelihood likelihood = model_to_likelihood(parent_dictionary, child_dictionary,
							  parser, a.scaling, p_corpus->begin(), p_corpus->end());
  a.print_latex(*p_corpus, likelihood, number_to_train_on, dictionary, parser);

  std::cout << "\n\n               FINISHED: " << a.comment  << "\n\n\n" << std::endl;
}

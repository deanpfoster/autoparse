//   -*- c++ -*-

//
//  This file is very close to learn.test.cc.  It can run very slowly, but
// learn.test should always run fast since it is used for testing.
//  Ideally, the code in both of these should live primarilly in learn.cc and not
//  in the learn.test.cc or learn.main.cc.
//

#include "learn.h"

// Might as well be random for production code, but until I want to run it twice
// we'll leave it on reproducible.
#define REPRODUCIBLE
// #define TRULY_RANDOM
#include "utilities/z.Template.h"

int
main(int argc,char** argv)
{
  time_t start_time = time(0);  // used for timing 
  time_t last_print_time = time(0);  // used to print about once a minute
  Eigen::initParallel();  // this will help eigen not walk on top of itself.
  std::ostream& debugging(std::cout);
  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    Which dictionary, corpus and latex file to use
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::string sentence_file = "sample_corpus";
  std::string eigen_file = "pretty_5_c_sample.csv";
  std::string latex_file = "learn.output.tex";
  int gram_number = 5;
  if(argc >= 3)
    {
      sentence_file = argv[1];
      eigen_file = argv[2];
      gram_number = 3;  // this is a guess
    }
  if(argc == 4)
    latex_file = argv[3];
  debugging << "  sentence = " << sentence_file << std::endl;
  debugging << "eigenwords = " << eigen_file << std::endl;
  debugging << "     latex = " << latex_file << std::endl;
  std::ofstream latex(latex_file);
  auto_parse::latex_header(latex);
      
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
    debugging << "Read a dictionary of size: " << dictionary.size()<< " x " << dim
	      << " (time " << time(0) - start_time << " sec)" << std::endl;      start_time = time(0);

    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            LR Parser setup
    //
    //////////////////////////////////////////////////////////////////////////////////

    auto_parse::Feature_generator feature_generator = standard_features(dictionary);
    auto_parse::Model lr_model = standard_model(feature_generator);
    auto_parse::Statistical_parse parser(lr_model);

    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            Likelihood setup
    //
    //////////////////////////////////////////////////////////////////////////////////

    auto_parse::TP_eigenwords tp(dictionary,dictionary);  
    auto_parse::Likelihood likelihood(tp,tp);

    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            MAIN LOOP
    //
    //////////////////////////////////////////////////////////////////////////////////
    std::cout << "Trained on " << corpus_in_memory.size() << " sentence.    " << std::endl;
    std::cout << "number threads = " << auto_parse::number_of_threads_used() << "." << std::endl;

    int n = corpus_in_memory.size();  // (About 462k)
    assert(n > 400*1000);
    int number_per_level = 30;
    std::vector<int> number_to_train_on(6*number_per_level, n);  // default = n
    int K = 1000;
    for(int i = 0; i < number_per_level;++i)
      number_to_train_on[i] = 10*K;
    for(int i = 1 * number_per_level; i < 2 * number_per_level;++i)
      number_to_train_on[i] = 20*K;
    for(int i = 2 * number_per_level; i < 3 * number_per_level;++i)
      number_to_train_on[i] = 40*K;
    for(int i = 3 * number_per_level; i < 4 * number_per_level;++i)
      number_to_train_on[i] = 100*K;
    for(int i = 4 * number_per_level; i < 5 * number_per_level;++i)
      number_to_train_on[i] = 200*K;
    
      
    for(unsigned int rounds = 0; rounds < number_to_train_on.size(); ++rounds)
      {
	std::vector<auto_parse::Words>::const_iterator begin = corpus_in_memory.begin();
	std::vector<auto_parse::Words>::const_iterator end   = begin + number_to_train_on[rounds];
	double sampling_rate = .05;  // this generates about a factor of 10 speed up by only computing X'X 5% of the time
	std::stringstream s;
	s << "  " << rounds << "       ";
	std::string debugging_prefix = s.str();

	///////////////////////////////////////////////
	//                                           //
	//           Likelihood --> Model            //
	//                                           //
	///////////////////////////////////////////////

	auto_parse::Model model = likelihood_to_model(likelihood, parser, feature_generator, sampling_rate, begin, end);
	parser.new_model(model);
	debugging << debugging_prefix << "Training time " << time(0) - start_time << " sec." << std::endl;      start_time = time(0);

	///////////////////////////////////////////////
	//                                           //
	//               EVALUATION                  //
	//                                           //
	///////////////////////////////////////////////

	std::vector<int> which_sentences(0);
	std::string summary =   evaluation(rounds, debugging, latex,
					   dictionary, parser, likelihood,
					   which_sentences, begin, end);
	debugging << debugging_prefix << " + + + + \t" << summary << "\t\t+ + + + + +" << std::endl;
	debugging << debugging_prefix << "Evaluation time " << time(0) - start_time << " sec." << std::endl;      start_time = time(0);
	
	///////////////////////////////////////////////
	//                                           //
	//  Model  --> Parsed corpus -->  MLE        //
	//                                           //
	///////////////////////////////////////////////

	likelihood = model_to_likelihood(parent_dictionary, child_dictionary, parser, begin, end);
	debugging << debugging_prefix << "MLE time " << time(0) - start_time << " sec." << std::endl;      start_time = time(0);

	///////////////////////////////////////////////
	//                                           //
	//               EVALUATION                  //
	//                                           //
	///////////////////////////////////////////////

	if(time(0) - last_print_time > 60)
	  {
	    which_sentences = std::vector<int>{0,2,4, 10, 17, 26};
	    last_print_time = time(0);
	  }
	summary =   evaluation(rounds, debugging, latex,
			       dictionary, parser, likelihood,
			       which_sentences, begin, end);
	debugging << debugging_prefix << " * * * * \t" << summary << "\t\t* * * * *" << std::endl;
	debugging << debugging_prefix << "Evaluation time " << time(0) - start_time << " sec." << std::endl;      start_time = time(0);

      }
    auto_parse::latex_footer(latex);
    debugging << "Finished!" << std::endl;
}

//   -*- c++ -*-

#include "learn.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"
//
//  This file is very close to learn.main.cc.  learn.main can run very slowly, but
//  learn.test should always run fast since it is used for testing.
//  Ideally, the code in both of these should live primarilly in learn.cc and not
//  in the learn.test.cc or learn.main.cc.  This ideal has only sort-of been met.
//

int
main()
{
  omp_set_num_threads(1); // this makes the code reproducable (otherwise random numbers are called in different orders.
  std::ostream& debugging(std::cout);
  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    Read which eigendictionary and corpus to use
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::string sentence_file = "sample_corpus";
  std::string eigen_file = "pretty_5_c_sample.csv";
  std::string latex_file = "learn.output.tex";
  int gram_number = 5;
  std::ofstream latex(latex_file);
  auto_parse::latex_header(latex);
  latex << "\\renewcommand{\\thepage}{runningTitle-\\arabic{page}}" << std::endl;  

      
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
    auto_parse::Eigenwords child_dictionary(dictionary); 
    int dim = dictionary.dimension();
    debugging << "Read a dictionary of size: " << dictionary.size()<< " x " << dim << std::endl;


    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            LR Parser setup
    //
    //////////////////////////////////////////////////////////////////////////////////

    auto_parse::Feature_generator feature_generator = fast_features(dictionary);
    auto_parse::Model lr_model = auto_parse::generate_linear_model(feature_generator.dimension());
    double noise = 1.0;
    auto_parse::Statistical_parse parser(lr_model,feature_generator,noise);

    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            Likelihood setup
    //
    //////////////////////////////////////////////////////////////////////////////////

    auto_parse::TP_eigenwords tp(dictionary,dictionary,1.0);  
    auto_parse::Likelihood likelihood(tp,tp,tp);

    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            MAIN LOOP
    //
    //////////////////////////////////////////////////////////////////////////////////
      
    for(int rounds = 0; rounds < 10; ++rounds)
      {
	///////////////////////////////////////////////
	//                                           //
	//           Likelihood --> Model            //
	//                                           //
	///////////////////////////////////////////////

	std::stringstream s;
	s << "  " << rounds << "    ";
	std::string debugging_prefix = s.str();
	// the following uses OpenMP to run faster
	double sampling_rate = 1.0;  // no need for speedup here
	debugging << debugging_prefix << "Training" << std::endl;

	auto_parse::Model new_model = likelihood_to_model(likelihood,
							  parser,
							  feature_generator,
							  sampling_rate,
							  corpus_in_memory.begin(),
							  corpus_in_memory.end(),
							  std::cout);
	parser.new_model(new_model);
	
	///////////////////////////////////////////////
	//                                           //
	//  Model  --> Parsed corpus -->  MLE        //
	//                                           //
	///////////////////////////////////////////////

	debugging << debugging_prefix << "MLE" << std::endl;
	likelihood = model_to_likelihood(parent_dictionary, child_dictionary,  parser, 1.0, corpus_in_memory.begin(), corpus_in_memory.end());
	debugging << debugging_prefix <<  likelihood << std::endl;

	///////////////////////////////////////////////
	//                                           //
	//               EVALUATION                  //
	//                                           //
	///////////////////////////////////////////////

	double sqrt_sum = 0;

#pragma omp parallel for 
	for(unsigned int i = 0; i <  corpus_in_memory.size(); ++i)
	  {
	    auto_parse::Words sentence = corpus_in_memory[i];
	    auto_parse::Dependency parse = redo_parse(sentence, parser(sentence)).parse();
	    double prob = likelihood(parse);
	    sqrt_sum += sqrt(fabs(prob));
	  };

	std::cout << debugging_prefix << " * * * * " << sqrt_sum << " * * * * " << std::endl;

	auto_parse::Dependency parse1 = redo_parse(corpus_in_memory[1], parser(corpus_in_memory[1])).parse();
	auto_parse::Dependency parse3 = redo_parse(corpus_in_memory[3], parser(corpus_in_memory[3])).parse();
	auto_parse::Dependency parse5 = redo_parse(corpus_in_memory[5], parser(corpus_in_memory[5])).parse();
	latex << "\\section{likelihood index: " << sqrt_sum << "}\n\n" << std::endl;
	parse1.latex(latex);
	parse3.latex(latex);
	parse5.latex(latex);
	std::cout << likelihood.decorate(parse1,dictionary) << std::endl;
	}
    auto_parse::latex_footer(latex);
    debugging << "Finished!" << std::endl;
}

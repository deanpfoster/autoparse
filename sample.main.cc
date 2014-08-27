//   -*- c++ -*-

#include "sample.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"


int
main(int argc,char** argv)
{
  time_t start_time = time(0);  // used for timing 
  std::ostream& debugging(std::cout);
  std::ofstream latex("sample.output.tex");
  auto_parse::latex_header(latex);
  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    Read which eigendictionary and corpus to use
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::string sentence_file = "sample_corpus";
  std::string eigen_file = "pretty_5_c_sample.csv";
  int gram_number = 5;
  if(argc == 3)
    {
      sentence_file = argv[1];
      eigen_file = argv[2];
      gram_number = 3;  // this is a guess
    }
  debugging << "  sentence = " << sentence_file << std::endl;
  debugging << "eigenwords = " << eigen_file << std::endl;
      
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

    Eigen::MatrixXd t = Eigen::MatrixXd::Identity(dim,dim); // This needs to be estimated
    auto_parse::TP_eigenwords tp(dictionary,t);  
    auto_parse::Likelihood likelihood(tp,tp);

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
	auto_parse::Model new_model = likelihood_to_model(likelihood,
							  parser,
							  feature_generator,
							  lr_model,
							  corpus_in_memory,
							  debugging,
							  debugging_prefix);
	parser.new_model(new_model);
	debugging << " (time " << time(0) - start_time << " sec " << std::endl;      start_time = time(0);
	
	///////////////////////////////////////////////
	//                                           //
	//  Model  --> Parsed corpus -->  MLE        //
	//                                           //
	///////////////////////////////////////////////

	debugging << debugging_prefix << "MLE" << std::endl;
	likelihood = model_to_likelihood( dictionary,
					  corpus_in_memory,
					  parser,
					  debugging,
					  debugging_prefix);

	debugging << " (time " << time(0) - start_time << " sec)" << std::endl;      start_time = time(0);

	///////////////////////////////////////////////
	//                                           //
	//               EVALUATION                  //
	//                                           //
	///////////////////////////////////////////////

	double sqrt_sum = 0;
	for(auto_parse::Words sentence : corpus_in_memory)
	  {
	    auto_parse::Dependency parse = redo_parse(sentence, parser(sentence)).parse();
	    double prob = likelihood(parse);
	    sqrt_sum += sqrt(fabs(prob));
	  }
	std::cout << debugging_prefix << " * * * * " << sqrt_sum << " * * * * " << std::endl;

	auto_parse::Dependency parse1 = redo_parse(corpus_in_memory[1], parser(corpus_in_memory[1])).parse();
	auto_parse::Dependency parse3 = redo_parse(corpus_in_memory[3], parser(corpus_in_memory[3])).parse();
	auto_parse::Dependency parse5 = redo_parse(corpus_in_memory[5], parser(corpus_in_memory[5])).parse();
	parse1.latex(latex);
	parse3.latex(latex);
	parse5.latex(latex);
	

	}
    auto_parse::latex_footer(latex);
    debugging << "Finished!" << std::endl;
}

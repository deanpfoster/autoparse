//   -*- c++ -*-

#include <iostream>
#include <assert.h>
#include <fstream>

#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"
#include "redo_parse.h"
#include "feature_generator.h"
#include "feature_one_dimensional.h"
#include "forecast_linear.h"
#include "eigenwords.h"
#include "tp_eigenwords.h"
#include "maximum_likelihood.h"
#include "feature_eigenwords.h"
#include "train_forecast_linear.h"
#include "row.h"
#include "contrast.h"
#include "tokenize.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"


namespace auto_parse
{
  Feature_generator
  standard_features(const Eigenwords& dictionary)
  {
    // the following uses new C++11 standard to shove all these into a vector
    std::vector<Feature*> features
        {
	  new Feature_one_dimensional<Words_left>,
	  new Feature_one_dimensional<Squared<Words_left> >,
	  new Feature_one_dimensional<Stack_size>,
	  new Feature_one_dimensional<Squared<Stack_size> >,
	  new Feature_one_dimensional<Sentence_length>,
	  new Feature_one_dimensional<Squared<Sentence_length> >,
	  new Feature_one_dimensional<Distance_to<0> >,
	  new Feature_one_dimensional<Distance_to<1> >,
	  new Feature_one_dimensional<Distance_to<2> >,
	  new Feature_eigenwords<Next_word>(dictionary),
	  new Feature_eigenwords<Stack_top>(dictionary),
	  new Feature_eigenwords<Stack_1>(dictionary)
	};
    Feature_generator result = Feature_generator(features);
    return result;
  };
  
  Model
  standard_model(const Feature_generator& feature_generator)
  {
    int num_features = feature_generator.dimension();
    Forecast* p_example_shift = new Forecast_linear(Eigen::VectorXd(num_features));
    Forecast* p_example_left  = new Forecast_linear(Eigen::VectorXd(num_features));
    Forecast* p_example_right = new Forecast_linear(Eigen::VectorXd(num_features));
    Forecast* p_example_reduce= new Forecast_linear(Eigen::VectorXd(num_features));
    Model result(
		 {   {Action::shift,       p_example_shift},
		     {Action::left_reduce, p_example_left},
		     {Action::right_reduce,p_example_right},
		     {Action::head_reduce, p_example_reduce}
		 },
		 feature_generator);
    return result;
  }
};

int
main(int argc,char** argv)
{
  std::string sentence_file = "sample_corpus";
  std::string eigen_file = "pretty_5_c_sample.csv";
  int gram_number = 5;
  if(argc == 3)
    {
      sentence_file = argv[1];
      eigen_file = argv[2];
      gram_number = 3;  // this is a guess
    }
  std::cout << "  sentence = " << sentence_file << std::endl;
  std::cout << "eigenwords = " << eigen_file << std::endl;
  {
      
    //////////////////////////////////////////////////////////////////////////////////
    //
    //                    Crude tokenizer and cruder input file!
    //
    //////////////////////////////////////////////////////////////////////////////////

    auto_parse::Tokenize corpus(sentence_file);
      
    //////////////////////////////////////////////////////////////////////////////////
    //
    //                    Eigenwords (used in both LR and likelihood)
    //
    //////////////////////////////////////////////////////////////////////////////////

    time_t start_time = time(0);
    std::ifstream in(eigen_file);
    auto_parse::Eigenwords dictionary(in,gram_number); 
    int dim = dictionary.dimension();
    std::cout << "Read a dictionary of size: " << dictionary.size()<< " x " << dim;
    std::cout << " (time " << time(0) - start_time << " sec)" << std::endl;      start_time = time(0);
    

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
      
    //    bool converged = false;
    //    while(!converged)
    std::vector<auto_parse::Words> corpus_in_memory;
    corpus.reset();
    while(!corpus.eof())
      corpus_in_memory.push_back(corpus.next_sentence());

    for(int rounds = 0; rounds < 10; ++rounds)
      {

	///////////////////////////////////////////////
	//                                           //
	//           Likelihood --> Model            //
	//                                           //
	///////////////////////////////////////////////

	std::map<auto_parse::Action, auto_parse::Train_forecast_linear> training;
	for(auto_parse::Action a: auto_parse::all_actions)
	  training[a] = auto_parse::Train_forecast_linear(lr_model.forecast(a));
	std::cout << "Training" << std::endl;


	std::vector<std::map<auto_parse::Action, auto_parse::Train_forecast_linear> > bundle(0);
	int num_threads;
	auto_parse::Contrast contrast(parser, likelihood, feature_generator);

#pragma omp parallel default(shared)
	{
#pragma omp single
	  {
	    num_threads = omp_get_num_threads();
	    bundle = std::vector<std::map<auto_parse::Action, auto_parse::Train_forecast_linear> >(num_threads);
	    for(int i = 0; i < num_threads; ++i)
	      bundle[i] = training;
	  }
	  
#pragma omp for 
	  for(unsigned int i = 0; i < corpus_in_memory.size(); ++i)
	    {
	    int thread_ID = omp_get_thread_num();
	    assert(thread_ID < num_threads);
	    auto sentence = corpus_in_memory[i];
	    std::vector<auto_parse::Row> contrast_pair = contrast(sentence);
	    for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	      bundle[thread_ID][i->m_a](i->m_X, i->m_Y);
	  };
#pragma omp single
	  {
	    for(int i = 1; i < num_threads;++i)
	      for(auto_parse::Action a: auto_parse::all_actions)
		bundle[0][a].merge(bundle[i][a]);

	    std::cout << "parsed " << corpus_in_memory.size() << " sentence.     (time " << time(0) - start_time << " sec * " << num_threads << " threads)" << std::endl;      start_time = time(0);

	    auto_parse::Model new_model(feature_generator);
	    for(auto_parse::Action a : auto_parse::all_actions)
	      new_model.add_forecast(a,bundle[0][a].result());
	    parser.new_model(new_model);
	  }
	  }

	///////////////////////////////////////////////
	//                                           //
	//  Model  --> Parsed corpus -->  MLE        //
	//                                           //
	///////////////////////////////////////////////

	std::cout << "MLE" << std::endl;

	auto_parse::TP_eigenwords left(dictionary,t); 
	auto_parse::TP_eigenwords right(dictionary,t); 
	auto_parse::Maximum_likelihood mle(left, right);
	for(unsigned int i = 0; i < corpus_in_memory.size(); ++i)
	  {	
	    auto sentence = corpus_in_memory[i];
	    auto_parse::Dependency d = redo_parse(sentence,parser(sentence)).parse();
	    mle(d);  // adds to database
	  }
	std::cout << "Finished redoing parse." << std::endl;
	likelihood = mle.output();
	std::cout << "Likelihood computed.     (time " << time(0) - start_time << " sec)" << std::endl;      start_time = time(0);

	std::cout << rounds << "   " <<  likelihood << std::endl;

	// Add some convergence criterion here
	//	converged = true;

      }
    std::cout << "Finished!" << std::endl;
  }
}

//   -*- c++ -*-

#include <iostream>
#include <assert.h>
#include <fstream>

#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"
#include "suggest_alternative_history.h"
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
	  new Feature_one_dimensional<Stack_size>,
	  new Feature_one_dimensional<Sentence_length>,
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
main()
{
  std::cout << "\n\n\n\t\t\t SAMPLE  SAMPLE  SAMPLE\n\n\n"<< std::endl;
  {
      
    //////////////////////////////////////////////////////////////////////////////////
    //
    //                    Eigenwords (used in both LR and likelihood)
    //
    //////////////////////////////////////////////////////////////////////////////////

    std::ifstream in("pretty_5_c_sample.csv");
    auto_parse::Eigenwords dictionary(in,5); 
    int dim = dictionary.dimension();

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

    Eigen::MatrixXd t = Eigen::MatrixXd::Zero(dim,dim); // This needs to be estimated
    for(int i = 0; i < dim; ++i)
      t(i,i) = 1;
    auto_parse::TP_eigenwords tp(dictionary,t);  
    auto_parse::Likelihood likelihood(tp,tp);


    //////////////////////////////////////////////////////////////////////////////////
    //
    //                            MAIN LOOP
    //
    //////////////////////////////////////////////////////////////////////////////////
      
    //    bool converged = false;
    //    while(!converged)
    for(int i = 0; i < 10; ++i)
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
	for(int i = 1; i < 100; ++i)
	  {
	    auto sentence = auto_parse::Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
	    auto_parse::Contrast contrast(parser, likelihood, feature_generator);
	    std::vector<auto_parse::Row> contrast_pair = contrast(sentence);
	    for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	      {
		for(auto_parse::Action a: auto_parse::all_actions)
		  {
		    if(i->m_a == a)
		      training[a](i->m_X, i->m_Y);
		  }
	      }
	  };
	auto_parse::Model new_model(feature_generator);
	for(auto_parse::Action a : auto_parse::all_actions)
	  new_model.add_forecast(a,training[a].result());
	parser.new_model(new_model);

	///////////////////////////////////////////////
	//                                           //
	//  Model  --> Parsed corpus -->  MLE        //
	//                                           //
	///////////////////////////////////////////////

	std::cout << "MLE" << std::endl;

	auto_parse::TP_eigenwords left(dictionary,t); 
	auto_parse::TP_eigenwords right(dictionary,t); 
	auto_parse::Maximum_likelihood mle(left, right);
	for(int i = 1; i < 100; ++i)
	  {
	    auto sentence = auto_parse::Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
	    auto_parse::Dependency d = redo_parse(sentence,parser(sentence)).parse();
	    mle(d);  // adds to database
	  }
	likelihood = mle.output();

	std::cout <<  likelihood << std::endl;

	// Add some convergence criterion here
	//	converged = true;

      }
  }
}

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
#include "feature_stack_size.h"
#include "feature_sentence_length.h"
#include "feature_words_left.h"
#include "forecast_constant.h"
#include "eigenwords.h"
#include "TP_eigenwords.h"
#include "maximum_likelihood.h"
#include "feature_eigenwords.h"

#include "contrast.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"


namespace auto_parse
{
  void test_sample()
  {
    std::cout << "\n\n\n\t\t\t SAMPLE  SAMPLE  SAMPLE\n\n\n"<< std::endl;
    {
      
      //////////////////////////////////////////////////////////////////////////////////
      //
      //                    Eigenwords (used in both LR and likelihood)
      //
      //////////////////////////////////////////////////////////////////////////////////

      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords eigenwords(in,5); 
      int dim = eigenwords.dimension();

      //////////////////////////////////////////////////////////////////////////////////
      //
      //                            LR Parser setup
      //
      //////////////////////////////////////////////////////////////////////////////////

      Feature_words_left f1;
      Feature_stack_size f2;
      Feature_sentence_length f3;
      Feature_eigenwords<Next_word> f4(eigenwords);
      Feature_eigenwords<Stack_top> f5(eigenwords);
      Feature_eigenwords<Stack_1> f6(eigenwords);
      Feature_generator feature_generator {&f1, &f2, &f3, &f4, &f5, &f6};
      Forecast_linear example();
      Model m(
      {   {Action::shift,&example},           // must have a shift to read words
	  {Action::left_reduce,&example},
	  {Action::right_reduce, &example},
	  {Action::head_reduce, &example}   // must have a head reduce to end a sentence
      },feature_generator);
      auto_parse::Statistical_parse parser(m);

      //////////////////////////////////////////////////////////////////////////////////
      //
      //                            Likelihood setup
      //
      //////////////////////////////////////////////////////////////////////////////////

      Eigen::MatrixXd t = Eigen::MatrixXd::Zero(dim,dim); // This needs to be estimated
      for(int i = 0; i < dim; ++i)
	t(i,i) = 1;
      auto_parse::TP_eigenwords tp(eigenwords,t);  
      auto_parse::Likelihood likelihood(tp,tp);


      //////////////////////////////////////////////////////////////////////////////////
      //
      //                            MAIN LOOP
      //
      //////////////////////////////////////////////////////////////////////////////////
      
      bool converged = false;
      while(!converged)
      {

	///////////////////////////////////////////////
	//                                           //
	//           Likelihood --> Model            //
	//                                           //
	///////////////////////////////////////////////

	std::map<Action, Train_forecast_linear> training;
	for(Action a: all_actions)
	  training[a] = Train_forecast_linear(m.forecast(a));

	for(int i = 1; i < 100; ++i)
	  {
	    auto sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
	    auto_parse::Contrast contrast(parser, likelihood, feature_generator);
	    std::vector<foo> contrast_pair = contrast(sentence);
	    for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	      {
		for(Action a: all_actions)
		  {
		    if(action_taken(*i) == a)
		      training[a](*i);
		  }
	      }
	  };
	Model new_model(feature_generator);
	for(a : all_actions)
	  new_model.add_forecast(a,training[a].result());
	parser.new_model(new_model);

	///////////////////////////////////////////////
	//                                           //
	//  Model  --> Parsed corpus -->  MLE        //
	//                                           //
	///////////////////////////////////////////////

	auto_parse::TP_eigenwords left(eigenwords,t); 
	auto_parse::TP_eigenwords right(eigenwords,t); 
	auto_parse::Maximum_likelihood mle(left, right);
	for(int i = 1; i < 100; ++i)
	  {
	    auto sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
	    Dependency d = redo_parse(sentence,parser(sentence)).parse();
	    mle(d);  // adds to database
	  }
	likelihood = mle.output();

	// Add some convergence criterion here
	converged = true;

      }
    }
  }
}
#ifndef NO_MAIN
int
main()
{
  auto_parse::test_sample();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


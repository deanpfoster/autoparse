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
      //                            LR Parser setup
      //
      //////////////////////////////////////////////////////////////////////////////////
      Feature_words_left f1;
      Feature_stack_size f2;
      Feature_sentence_length f3;
      Feature_generator feature_generator {&f1, &f2, &f3};
      Forecast_constant example(10.0);
      Model m(
      {   {Action::shift,&example},
	  {Action::left_reduce,&example},
	  {Action::right_reduce, &example},
	  {Action::head_reduce, &example}
      },feature_generator);
      auto_parse::Statistical_parse parser(m);

      //////////////////////////////////////////////////////////////////////////////////
      //
      //                            Likelihood setup
      //
      //////////////////////////////////////////////////////////////////////////////////
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5); 
      int dim = g.dimension();
      Eigen::MatrixXd t = Eigen::MatrixXd::Zero(dim,dim); // This needs to be estimated
      for(int i = 0; i < dim; ++i)
	t(i,i) = 1;
      auto_parse::TP_eigenwords tp(g,t);  
      auto_parse::Likelihood likelihood(tp,tp);


      {
	auto sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";

	// And again using the contrast class

	auto_parse::Contrast contrast(parser, likelihood, feature_generator);
	contrast(std::cout, sentence);

	std::cout << "constructed!" << std::endl;
      };
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


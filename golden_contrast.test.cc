//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"
#include "redo_parse.h"
#include "feature_generator.h"
#include "feature_one_dimensional.h"
#include "forecast_constant.h"
#include <fstream>
#include "eigenwords.h"
#include "tp_eigenwords.h"
#include "tp_iid.h"
#include "row.h"

#include "golden_contrast.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"


namespace auto_parse
{
  void test_golden_contrast()
  {
    std::cout << "\n\n\n\t\t\t GOLDEN_CONTRAST  GOLDEN_CONTRAST  GOLDEN_CONTRAST\n\n\n"<< std::endl;
    {
      Forecast_constant example(10.0);
      Feature_generator gen;
      Model m(
      {   {Action::shift,&example},
	  {Action::left_reduce,&example},
	  {Action::right_reduce, &example},
	  {Action::head_reduce, &example}
      });
      double noise = 1.0;
      auto_parse::Statistical_parse parser(m,gen,noise);
      Feature_one_dimensional<Words_left> f1;
      Feature_one_dimensional<Stack_size> f2;
      Feature_one_dimensional<Sentence_length> f3;
      Feature_generator feature_generator;
      feature_generator.add({&f1, &f2, &f3});

      Golden_contrast golden_contrast(parser, feature_generator);

	std::cout << "constructed!" << std::endl;
      };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_golden_contrast();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


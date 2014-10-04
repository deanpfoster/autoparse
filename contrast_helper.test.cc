//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "model.h"
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

#include "contrast_helper.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"


namespace auto_parse
{
  void test_constrast_helper()
  {
    std::cout << "\n\n\n\t\t\t CONSTRAST_HELPER  CONSTRAST_HELPER  CONSTRAST_HELPER\n\n\n"<< std::endl;
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
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      int dim = g.dimension();
      Eigen::MatrixXd matrix = Eigen::MatrixXd::Random(dim,dim);
      std::vector<double> prob(20,.05);
      auto_parse::TP_eigenwords markov(g,g,1.0);  // testing construction
      auto_parse::TP_iid root(g.size(),1.0);  // testing construction
      Feature_one_dimensional<Words_left> f1;
      Feature_one_dimensional<Stack_size> f2;
      Feature_one_dimensional<Sentence_length> f3;
      Feature_generator feature_generator;
      feature_generator.add({&f1, &f2, &f3});
      {
      Lexicon lexicon {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
	auto sentence = Words(&lexicon) + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";

	History h = parser(sentence);

	Contrast_helper contrast_helper(parser, feature_generator);


	std::cout << "constructed!" << std::endl;
      };
    }
  }
}
#ifndef NO_MAIN
int
main()
{
  auto_parse::test_constrast_helper();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


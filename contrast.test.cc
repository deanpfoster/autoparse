//   -*- c++ -*-


#include <iostream>
#include <assert.h>

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
#include <fstream>
#include "eigenwords.h"
#include "tp_eigenwords.h"

#include "contrast.h"

#define REPRODUCIBLE
#include "utilities/z.Template.h"


namespace auto_parse
{
  void test_contrast()
  {
    std::cout << "\n\n\n\t\t\t CONTRAST  CONTRAST  CONTRAST\n\n\n"<< std::endl;
    {
      Forecast_constant example(10.0);
      Feature_generator gen;
      Model m(
      {   {Action::shift,&example},
	  {Action::left_reduce,&example},
	  {Action::right_reduce, &example},
	  {Action::head_reduce, &example}
      },gen);
      auto_parse::Statistical_parse parser(m);
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      int dim = g.dimension();
      Eigen::MatrixXd matrix = Eigen::MatrixXd::Random(dim,dim);
      auto_parse::TP_eigenwords markov(g,matrix);  // testing construction
      Likelihood likelihood(markov,markov);
      Feature_words_left f1;
      Feature_stack_size f2;
      Feature_sentence_length f3;
      Feature_generator feature_generator {&f1, &f2, &f3};
      {
	auto sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";

	

	Statistical_history h = parser(sentence);
	History prefix = suggest_alternative_history(h);  // truncates and modifies the history
	History h_prime = parser.finish(sentence, prefix);
	double l       = likelihood(redo_parse(sentence, h      ).parse());
	double l_prime = likelihood(redo_parse(sentence, h_prime).parse());
	History common = prefix;
	common.pop_back();
	std::cout << "  common: " << common << std::endl;
	std::cout << "original: " << h << std::endl;
	std::cout << " new seq: " << h_prime << std::endl;
	std::cout << "  prefix: " << prefix << std::endl;
	Action a_prime = h_prime[common.size()];
	Action a = h[common.size()];
	std::cout << "    last: " << a << std::endl;
	std::cout << "new last: " << a_prime << std::endl;
	assert(a != a_prime);
	write_row(std::cout, feature_generator, sentence, common,
		 a, l, a_prime, l_prime);
	
	// And again using the contrast class

	Contrast contrast(parser, likelihood, feature_generator);
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
  auto_parse::test_contrast();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


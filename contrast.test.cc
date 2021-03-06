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
      Likelihood likelihood(*markov.renormalize(),*markov.renormalize(),root);
      Feature_one_dimensional<Words_left> f1;
      Feature_one_dimensional<Stack_size> f2;
      Feature_one_dimensional<Sentence_length> f3;
      Feature_generator feature_generator;
      feature_generator.add({&f1, &f2, &f3});
      {
      Lexicon lexicon {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
	auto sentence = Words(&lexicon) + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";

	

	History h = parser(sentence);

	Contrast contrast(parser, likelihood, feature_generator);

	History prefix;
	while(prefix.size() == 0)
	  prefix = contrast.suggest_alternative_history(sentence,h);  // truncates and modifies the history
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
	std::cout << rows(feature_generator, sentence, common, a, l, a_prime, l_prime);
	
	// And again using the contrast class

	std::cout << contrast(sentence);

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


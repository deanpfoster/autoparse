//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"

// #include "contrast.h"

namespace auto_parse
{
  void test_contrast()
  {
    std::cout << "\n\n\n\t\t\t CONTRAST  CONTRAST  CONTRAST\n\n\n"<< std::endl;
    {
      Model m;
      auto_parse::Statistical_parse parser(m);
      Transition_probability markov;
      Likelihood likelihood(markov,markov);
      {
	auto sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
	Statistical_history h = parser(sentence);
	History prefix = suggest_alternative_prefix(h);  // truncates and modifies the history
	History h_prime = parser(sentence, prefix);
	double l       = likelihood(redo_parse(sentence, h      ));
	double l_prime = likelihood(redo_parse(sentence, h_prime));
	cout << parser.print_row(sentence, history, l, prefix, l_prime) << std::endl;
      }
      

      std::cout << "constructed!" << std::endl;
    };
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


//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "likelihood.h"

namespace auto_parse
{
  void test_likelihood()
  {
    std::cout << "\n\n\n\t\t\t LIKELIHOOD  LIKELIHOOD  LIKELIHOOD\n\n\n"<< std::endl;
    {
      Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
      auto_parse::Transition_probability left;
      auto_parse::Transition_probability right;
      auto_parse::Likelihood lambda(left,right); 
      std::cout << lambda << std::endl;
      typedef auto_parse::Dependency D;
      D complex =  (D("A") < D("hearing") > (D("on") > (D("the") < D("issue"))))
	< ((D("is") > (D("scheduled") > D("today"))) > D("."));
      std::cout << lambda(complex) << std::endl;
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_likelihood();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


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
      auto_parse::Transition_probability left;
      auto_parse::Transition_probability right;
      auto_parse::Likelihood lambda(left,right); 
      std::cout << lambda << std::endl;
      typedef auto_parse::Dependency D;
      D complex =  (D("A") < D("hearing") > (D("on") > (D("the") < D("issue"))))
	< ((D("is") > (D("scheduled") > D("today"))) > D("."));
      std::cout << lambda(complex) << std::endl;
      std::cout << "constructed!" << std::endl;

      D sorted = D("A") < D("B") < D("C") < D("D") < D("E");
      std::cout << sorted;
      std::cout << "Sorted: " << lambda(sorted) << std::endl;
      D r = D("A") > D("B") > D("C") > D("D") > D("E");
      std::cout << r;
      std::cout << "right hanging: " << lambda(r) << std::endl;
      D reverse = D("E") < D("D") < D("C") < D("B") < D("A");
      std::cout << reverse;
      std::cout << "reverse: " << lambda(reverse) << std::endl;
      

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


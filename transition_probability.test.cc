//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "transition_probability.h"

namespace auto_parse
{
  void test_transition_probability()
  {
    std::cout << "\n\n\n\t\t\t TRANSITION_PROBABILITY  TRANSITION_PROBABILITY  TRANSITION_PROBABILITY\n\n\n"<< std::endl;
    {
      auto_parse::Transition_probability t;  // testing construction
      std::cout << "constructed!" << std::endl;
      std::cout << t("A", "B") << " should be 2/3" << std::endl;
      std::cout << t("B", "A") << " should be 1/3" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_transition_probability();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


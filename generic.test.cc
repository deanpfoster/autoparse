//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "generic.h"

namespace auto_parse
{
  void test_generic()
  {
    std::cout << "\n\n\n\t\t\t GENERIC  GENERIC  GENERIC\n\n\n"<< std::endl;
    {
      auto_parse::Generic g;  // testing construction
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_generic();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


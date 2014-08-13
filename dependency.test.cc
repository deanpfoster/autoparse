//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "dependency.h"

namespace auto_parse
{
  void test_Dependency()
  {
    std::cout << "\n\n\n\t\t\t DEPENDENCY  DEPENDENCY  DEPENDENCY\n\n\n"<< std::endl;
    {
      auto_parse::Dependency g;  // testing construction
      std::cout << "constructed!" << std::endl;
      std::cout << g;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  //  auto_parse::test_generic();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


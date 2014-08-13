//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "generic.h"

namespace library
{
  void test_generic()
  {
    std::cout << "\n\n\n\t\t\t GENERIC  GENERIC  GENERIC\n\n\n"<< std::endl;
    {
      library::Generic g;  // testing construction
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  library::test_generic();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


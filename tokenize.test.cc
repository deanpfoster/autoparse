//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "tokenize.h"

namespace auto_parse
{
  void test_tokenize()
  {
    std::cout << "\n\n\n\t\t\t TOKENIZE  TOKENIZE  TOKENIZE\n\n\n"<< std::endl;
    {
      auto_parse::Tokenize in("tokenize.test.cc");  // testing construction
      while(!in.eof())
	{
	  std::cout << in.next_sentence() << std::endl;
	}
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_tokenize();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


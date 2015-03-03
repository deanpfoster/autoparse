//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "tokenize.h"

namespace auto_parse
{
  void test_tokenize()
  {
    std::cout << " \n\n\n\t\t\t TOKENIZE  TOKENIZE  TOKENIZE \n \n \n " << std::endl;
    {
      Lexicon l {"<OOV>", "iostream", "#include", "namespace", "std", "while",
	  "auto", "parse", "{" , "}","std::cout", "std::endl;","tokenize","\\n", "," "\"", "QUOTE",
	  "main()","int","while","<<"};
      auto_parse::Tokenize in("tokenize.test.cc",&l);  // testing construction
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


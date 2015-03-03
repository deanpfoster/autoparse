//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "lexicon_generator.h"
#include "tokenize.h"

namespace auto_parse
{
  void test_lexicon_generator()
  {
    std::cout << "\n\n\n\t\t\t LEXICON_GENERATOR  LEXICON_GENERATOR  LEXICON_GENERATOR\n\n\n"<< std::endl;
    {
      auto_parse::Lexicon_generator lexicon;  // testing construction
      std::cout << "Lexicon start off at: " << lexicon.size() << std::endl; 
      auto_parse::Tokenize in("tokenize.test.cc",&lexicon);
      std::cout << "constructed!" << std::endl;
      while(!in.eof())
	{
	  std::cout << in.next_sentence() << std::endl;
	}
      std::cout << "Lexicon is now: " << lexicon.size(); 
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_lexicon_generator();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


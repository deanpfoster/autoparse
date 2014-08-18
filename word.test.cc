//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "word.h"

namespace auto_parse
{
  void test_word()
  {
    std::cout << "\n\n\n\t\t\t WORD   WORD   WORD\n\n\n"<< std::endl;
    {
      Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
      std::cout << "constructed!" << std::endl;

      std::cout << w << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_word();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


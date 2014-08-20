//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "history.h"

namespace auto_parse
{
  void test_history()
  {
    std::cout << "\n\n\n\t\t\t HISTORY  HISTORY  HISTORY\n\n\n"<< std::endl;
    {
      auto_parse::History h;
      h.push_back(auto_parse::Actions::shift);
      h.push_back(auto_parse::Actions::left_reduce);
      h.push_back(auto_parse::Actions::right_reduce);
      h.push_back(auto_parse::Actions::head_reduce);
      std::cout << h << std::endl;
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_history();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


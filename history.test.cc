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
      h.push_back(auto_parse::Action::shift);
      h.push_back(auto_parse::Action::left_reduce);
      h.push_back(auto_parse::Action::right_reduce);
      h.push_back(auto_parse::Action::head_reduce);
      std::cout << h << std::endl;
      std::cout << "constructed!" << std::endl;

      auto_parse::Action i = static_cast<auto_parse::Action>(10);
      int j = static_cast<int>(auto_parse::Action::shift);
      std::cout << "\t 10 = " << i << "\n\t shift = "  << j << std::endl;
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


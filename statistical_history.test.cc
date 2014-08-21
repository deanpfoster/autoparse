//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "statistical_history.h"

namespace auto_parse
{
  void test_statistical_history()
  {
    std::cout << "\n\n\n\t\t\t STATISTICAL_HISTORY  STATISTICAL_HISTORY  STATISTICAL_HISTORY\n\n\n"<< std::endl;
    {
      auto_parse::Statistical_history h;
      Value_of_forecasts v;
      h.push_back(auto_parse::Action::shift,v);
      h.push_back(auto_parse::Action::left_reduce,v);
      h.push_back(auto_parse::Action::right_reduce,v);
      h.push_back(auto_parse::Action::head_reduce,v);
      std::cout << h << std::endl;
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_statistical_history();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


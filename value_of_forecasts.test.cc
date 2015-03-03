//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "value_of_forecasts.h"
#define REPRODUCIBLE
#include "utilities/z.Template.h"


namespace auto_parse
{
  void test_value_of_forecasts()
  {
    std::cout << "\n\n\n\t\t\t VALUE_OF_FORECASTS  VALUE_OF_FORECASTS  VALUE_OF_FORECASTS\n\n\n"<< std::endl;
    {
      auto_parse::Value_of_forecasts g;  // testing construction
      g[Action::shift] = 3.0;
      g[Action::right_reduce] = 4.0;
      std::cout << g[Action::shift] << std::endl;
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_value_of_forecasts();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


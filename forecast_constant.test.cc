//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include "forecast_constant.h"


namespace auto_parse
{

  struct LR{};

  void test_forecast_constant()
  {
    std::cout << "\n\n\n\t\t\t FORECAST_CONSTANT  FORECAST_CONSTANT  FORECAST_CONSTANT\n\n\n"<< std::endl;

    Forecast_constant::register_forecast(new Forecast_constant);
    {
      Forecast_constant example(10);
      std::stringstream s;
      s << example;  // save the example
      std::cout << example << std::endl;
      Forecast* p_forecast;
      s >> p_forecast;
      std::cout << *p_forecast << std::endl; // confirm that it writes the same thing as before

      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_forecast_constant();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include "forecast_linear.h"


namespace auto_parse
{

  struct LR{};

  void test_forecast_linear()
  {
    std::cout << "\n\n\n\t\t\t FORECAST_LINEAR  FORECAST_LINEAR  FORECAST_LINEAR\n\n\n"<< std::endl;

    Forecast_linear::register_forecast(new Forecast_linear);
    {
      auto_parse::Vector beta(3);
      beta << 1. ,  2. , 3.;
      Forecast_linear example(beta);

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
  auto_parse::test_forecast_linear();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


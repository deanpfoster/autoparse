//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "train_forecast_linear.h"

namespace auto_parse
{
  void test_train_forecast_linear()
  {
    std::cout << "\n\n\n\t\t\t TRAIN_FORECAST_LINEAR  TRAIN_FORECAST_LINEAR  TRAIN_FORECAST_LINEAR\n\n\n"<< std::endl;
    {
      Forecast_linear example;
      Train_forecast_linear training(example);


      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_train_forecast_linear();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


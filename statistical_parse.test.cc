//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>

#include "statistical_parse.h"
#include "forecast_constant.h"
#define REPRODUCIBLE
#include "utilities/z.Template.h"

namespace auto_parse
{
  void test_statistical_parse()
  {
    std::cout << "\n\n\n\t\t\t STATISTICAL_PARSE  STATISTICAL_PARSE  STATISTICAL_PARSE\n\n\n"<< std::endl;
    {
      Forecast_constant example(10);
      Model m;
      for(Action a : eager_actions)
	{
	  m.add_forecast(a,example);
	};
      Feature_generator fg;
      double noise = .1;
      auto_parse::Statistical_parse g(m,fg,noise);  // testing construction
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_statistical_parse();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


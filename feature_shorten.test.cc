//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <iterator>

#include "feature_shorten.h"
#include "eigenwords.h"
#include "feature_eigenwords.h"

std::ostream & operator<<(std::ostream & ostrm, const std::vector<std::string> & variables)
{
  for(auto w: variables)
    ostrm << w << " : ";
  return ostrm;
};



namespace auto_parse
{
  void test_shorten()
  {
    std::cout << "\n\n\n\t\t\t (feature_)SHORTEN  feature_SHORTEN  feature_SHORTEN\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      Feature_eigenwords<Stack_top>  e(g);

      Shorten s(e,10);
      std::cout << s.variable_names() << std::endl;
      std::cout << s.dimension() << std::endl;
      std::cout << s.name() << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_shorten();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include <iterator>

#include "feature_eigenwords.h"

namespace auto_parse
{
  void test_feature_eigenwords()
  {
    std::cout << "\n\n\n\t\t\t FEATURE_EIGENWORDS  FEATURE_EIGENWORDS  FEATURE_EIGENWORDS\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      Feature_eigenwords<Stack_top>  s(g);
      std::cout << s.name() << std::endl;
      std::cout << s.dimension() << std::endl;
      std::vector<std::string> names(s.dimension());
      s.set_names(names.begin());
      std::copy(names.begin(), names.end(), std::ostream_iterator<std::string>(std::cout," "));
      std::cout << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_feature_eigenwords();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


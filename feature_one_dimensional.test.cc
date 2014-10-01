//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include <iterator>

#include "feature_one_dimensional.h"

std::ostream & operator<<(std::ostream & ostrm, const std::vector<std::string> & variables)
{
  for(auto w: variables)
    ostrm << w << " : ";
  return ostrm;
};

namespace auto_parse
{
  void test_feature_one_dimensional()
  {
    std::cout << "\n\n\n\t\t\t FEATURE_ONE_DIMENSIONAL<T>  FEATURE_ONE_DIMENSIONAL<T>  FEATURE_ONE_DIMENSIONAL<T>\n\n\n"<< std::endl;
    {
      Feature_one_dimensional<Words_left> s1;
      Feature_one_dimensional<Sentence_length> s2;
      Feature_one_dimensional<Stack_size> s3;
      std::cout << s1.name() << std::endl;
      std::cout << s2.name() << std::endl;
      std::cout << s3.name() << std::endl;
      std::cout << s1.dimension() << std::endl;
      std::cout << s2.dimension() << std::endl;
      std::cout << s3.dimension() << std::endl;
      std::cout << s1.variable_names();
      std::cout << s2.variable_names();
      std::cout << s3.variable_names();
      std::cout << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_feature_one_dimensional();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include <iterator>

#include "feature_words_left.h"

namespace auto_parse
{
  void test_feature_words_left()
  {
    std::cout << "\n\n\n\t\t\t FEATURE_WORDS_LEFT  FEATURE_WORDS_LEFT  FEATURE_WORDS_LEFT\n\n\n"<< std::endl;
    {
      Feature_words_left s;
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
  auto_parse::test_feature_words_left();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


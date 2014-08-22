//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include <iterator>

#include "feature_sentence_length.h"

namespace auto_parse
{
  void test_feature_sentence_length()
  {
    std::cout << "\n\n\n\t\t\t FEATURE_SENTENCE_LENGTH  FEATURE_SENTENCE_LENGTH  FEATURE_SENTENCE_LENGTH\n\n\n"<< std::endl;
    {
      Feature_sentence_length s;
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
  auto_parse::test_feature_sentence_length();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


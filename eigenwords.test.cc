//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "eigenwords.h"
#include <fstream>

namespace auto_parse
{
  void test_eigenwords()
  {
    std::cout << "\n\n\n\t\t\t EIGENWORDS  EIGENWORDS  EIGENWORDS\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      std::cout << g[Word("foo")] << std::endl << std::endl;
      std::cout << g[Word("and")] << std::endl;
      Eigenwords root = Eigenwords::create_root_dictionary();
      std::cout << root[Word("foo")] << std::endl;
      Words sentence;
      std::cout << root(sentence.end(),sentence) << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_eigenwords();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


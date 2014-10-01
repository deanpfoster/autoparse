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
      Eigenwords g(in,5);  // testing construction
      Lexicon l = g.lexicon();
      std::cout << g[Word(l, "foo")].norm() << " " << g[Word(l, "and")].norm() << std::endl;
      Eigenwords h(g);
      std::cout << h[Word(l, "foo")].norm() << " " << h[Word(l, "and")].norm() << std::endl;
      Eigenwords* p_1 = new Eigenwords(h);
      Eigenwords* p_2 = new Eigenwords(h);
      Eigenwords* p_3 = new Eigenwords(h);
      delete p_1;
      delete p_2;
      std::cout << h[Word(l, "foo")].norm() << " " << h[Word(l, "and")].norm() << std::endl;      
      std::cout << (*p_3)[Word(l, "foo")].norm() << " " << (*p_3)[Word(l, "and")].norm() << std::endl;      

      Eigenwords root = Eigenwords::create_root_dictionary(l);
      std::cout << h[Word(l, "foo")].norm() << " " << h[Word(l, "and")].norm() << std::endl;      

      std::cout << root[Word(l, "foo")].norm() << " " << root[Word(l, "and")].norm() << std::endl;

      std::cout << root[Word(root.lexicon(), "foo")] << std::endl;
      Words sentence(&l);
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


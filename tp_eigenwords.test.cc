//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "tp_eigenwords.h"
#include "eigenwords.h"

namespace auto_parse
{
  void test_tp_eigenwords()
  {
    std::cout << "\n\n\n\t\t\t TP_EIGENWORDS  TP_EIGENWORDS  TP_EIGENWORDS\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      int dim = g.dimension();
      Eigen::MatrixXd parent = Eigen::MatrixXd::Random(dim,dim);
      Eigen::MatrixXd child = Eigen::MatrixXd::Random(dim,dim);
      std::vector<double> prob(20,.05);
      auto_parse::TP_eigenwords tp(g, g, parent, child, 1, prob);  // testing construction
      Lexicon l {"<OOV>", "", "a", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      Words words(l,"");
      words.push_back(Word(l,"<OOV>"));
      words.push_back(Word(l,"the"));
      words.push_back(Word(l,"the"));
      Words::const_iterator a = words.begin();
      Words::const_iterator b = words.begin();
      ++b;
      Words::const_iterator c = b;
      ++c;
      std::cout << tp(a,b,words) << std::endl;
      std::cout << tp(b,a,words) << std::endl;
      std::cout << tp(b,c,words) << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_tp_eigenwords();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


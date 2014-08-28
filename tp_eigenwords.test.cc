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
      Eigen::MatrixXd t = Eigen::MatrixXd::Random(dim,dim);
      auto_parse::TP_eigenwords tp(g,g,t);  // testing construction
      std::cout << tp("<OOV>", "<OOV>") << std::endl;
      std::cout << tp("the", "<OOV>") << std::endl;
      std::cout << tp("the", "the") << std::endl;
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


//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "maximum_likelihood.h"
#include "tp_eigenwords.h"
#include "likelihood.h"


namespace auto_parse
{
  void test_maximum_likelihood()
  {
    std::cout << "\n\n\n\t\t\t MAXIMUM_LIKELIHOOD  MAXIMUM_LIKELIHOOD  MAXIMUM_LIKELIHOOD\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      int dim = g.dimension();
      Eigen::MatrixXd parent = Eigen::MatrixXd::Random(dim,dim);
      Eigen::MatrixXd child = Eigen::MatrixXd::Random(dim,dim);
      std::vector<double> prob(20,.05);
      auto_parse::TP_eigenwords left(g,g,parent,child,prob);  // testing construction
      auto_parse::TP_eigenwords right(g,g);  // testing construction
      auto_parse::Maximum_likelihood lambda(left,right); 
      typedef auto_parse::Dependency D;
      D complex =  (D("A") < D("hearing") > (D("on") > (D("the") < D("issue"))))
	< ((D("is") > (D("scheduled") > D("today"))) > D("."));
      lambda(complex);
      D sorted = D("A") < D("B") < D("C") < D("D") < D("E");
      lambda(sorted);
      D r = D("A") > D("B") > D("C") > D("D") > D("E");
      lambda(r);
      D reverse = D("E") < D("D") < D("C") < D("B") < D("A");
      lambda(reverse);
      std::cout << lambda.output();
    };
  }
  
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_maximum_likelihood();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


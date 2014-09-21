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
      auto_parse::Eigenwords dictionary(in,5);  // testing construction
      int dim = dictionary.dimension();
      Eigen::MatrixXd parent = Eigen::MatrixXd::Random(dim,dim);
      Eigen::MatrixXd child = Eigen::MatrixXd::Random(dim,dim);
      std::vector<double> prob(20,.05);
      auto_parse::TP_eigenwords left(dictionary,dictionary,parent,child,1,prob);  // testing construction
      auto_parse::TP_eigenwords right(dictionary,dictionary,1.0);  // testing construction
      auto_parse::TP_eigenwords root(Eigenwords::create_root_dictionary(),dictionary,1.0);  // testing construction
      auto_parse::Maximum_likelihood lambda(left,right,root); 
      typedef auto_parse::Dependency D;
      D complex =  (D(Word("A")) < D(Word("hearing")) > (D(Word("on")) > (D(Word("the")) < D(Word("issue")))))
	< ((D(Word("is")) > (D(Word("scheduled")) > D(Word("today")))) > D(Word(".")));
      lambda(complex);
      D sorted = D(Word("A")) < D(Word("B")) < D(Word("C")) < D(Word("D")) < D(Word("E"));
      lambda(sorted);
      D r = D(Word("A")) > D(Word("B")) > D(Word("C")) > D(Word("D")) > D(Word("E"));
      lambda(r);
      D reverse = D(Word("E")) < D(Word("D")) < D(Word("C")) < D(Word("B")) < D(Word("A"));
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


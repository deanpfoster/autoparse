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
      auto_parse::TP_eigenwords left(dictionary,dictionary,parent,child,1,prob);  
      auto_parse::TP_eigenwords right(dictionary,dictionary,1.0);  
      auto_parse::TP_eigenwords root(Eigenwords::create_root_dictionary(dictionary.lexicon()),dictionary,1.0);  
      auto_parse::Maximum_likelihood lambda(left,right,root); 
      typedef auto_parse::Dependency D;
      const Lexicon& l = dictionary.lexicon();
      D complex =  (D(l,"a") < D(l,"home") > (D(l,"on") > (D(l,"the") < D(l,"can"))))
	< ((D(l,"is") > (D(l,"for") > D(l,"you"))) > D(l,"."));
      lambda(complex);
      D sorted = D(l,"A") < D(l,"B") < D(l,"C") < D(l,"D") < D(l,"E");
      lambda(sorted);
      D r = D(l,"A") > D(l,"B") > D(l,"C") > D(l,"D") > D(l,"E");
      lambda(r);
      D reverse = D(l,"E") < D(l,"D") < D(l,"C") < D(l,"B") < D(l,"A");
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


//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "likelihood.h"
#include "tp_eigenwords.h"
#include "tp_iid.h"

namespace auto_parse
{
  void test_likelihood()
  {
    std::cout << "\n\n\n\t\t\t LIKELIHOOD  LIKELIHOOD  LIKELIHOOD\n\n\n"<< std::endl;
    typedef auto_parse::Dependency D;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords dictionary(in,5);  // testing construction
      const Lexicon& l = dictionary.lexicon();
      int v = dictionary.size();
      int dim = dictionary.dimension();
      Eigen::MatrixXd parent = Eigen::MatrixXd::Random(dim,dim);
      Eigen::MatrixXd child = Eigen::MatrixXd::Random(dim,dim);
      std::vector<double> prob(20,.05);
      std::vector<double> out_degree(v,.5);

      auto_parse::TP_eigenwords left(dictionary, dictionary, parent, child, 1, prob, out_degree);  // testing construction
      auto_parse::TP_eigenwords right(dictionary, dictionary, parent, child, 1, prob, out_degree);  // testing construction
      auto_parse::TP_iid root(v,1);
      auto_parse::Likelihood lambda(left,right,*root.renormalize()); 
      std::cout << lambda << std::endl;
      D complex =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
	< ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,"."));
      std::cout << lambda(complex) << std::endl;
      std::cout << "constructed!" << std::endl;
    }
    {
      Lexicon l {"<OOV>", "A", "B", "C", "D", "E"};
      Eigenwords dictionary = Eigenwords::create_root_dictionary(l);
      TP_iid root(l.size(),1.0);
      auto_parse::Likelihood lambda(root,root,root); 
      D sorted = D(l,"A") < D(l,"B") < D(l,"C") < D(l,"D") < D(l,"E");
      std::cout << sorted;
      std::cout << "Sorted: " << lambda(sorted) << std::endl;
      D r = D(l,"A") > D(l,"B") > D(l,"C") > D(l,"D") > D(l,"E");
      std::cout << r;
      std::cout << "right hanging: " << lambda(r) << std::endl;
      D reverse = D(l,"E") < D(l,"D") < D(l,"C") < D(l,"B") < D(l,"A");
      std::cout << reverse;
      std::cout << "reverse: " << lambda(reverse) << std::endl;
      std::cout << "Pretty version: " << lambda.decorate(reverse,dictionary) << std::endl;
      std::cout << lambda.summarize_pieces(lambda.pieces(reverse)) << std::endl;
    };
  }
  
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_likelihood();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


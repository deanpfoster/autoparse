//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "likelihood.h"
class sample: public auto_parse::Transition_probability
{ public:
  ~sample(){};
  sample(){};
  sample* clone() const  { return new sample(*this);};
  void accumulate(const auto_parse::Node&, const auto_parse::Node&, const auto_parse::Words&){};
  Transition_probability* renormalize() const{return 0;};
  double operator()(const auto_parse::Node& parent,const auto_parse::Node& child, const auto_parse::Words&) const
  {    if(parent < child) return 1.0; else return 0.0; };
  void print_on(std::ostream & ostrm) const
  { ostrm << "Heap: left is less than right." << std::endl;  };
};

namespace auto_parse
{
  void test_likelihood()
  {
    std::cout << "\n\n\n\t\t\t LIKELIHOOD  LIKELIHOOD  LIKELIHOOD\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords dictionary(in,5);  // testing construction

      sample left;
      sample right;
      sample root;
      auto_parse::Likelihood lambda(left,right,root); 
      std::cout << lambda << std::endl;
      typedef auto_parse::Dependency D;
      D complex =  (D(Word("A")) < D(Word("hearing")) > (D(Word("on")) > (D(Word("the")) < D(Word("issue")))))
	< ((D(Word("is")) > (D(Word("scheduled")) > D(Word("today")))) > D(Word(".")));
      std::cout << lambda(complex) << std::endl;
      std::cout << "constructed!" << std::endl;

      D sorted = D(Word("A")) < D(Word("B")) < D(Word("C")) < D(Word("D")) < D(Word("E"));
      std::cout << sorted;
      std::cout << "Sorted: " << lambda(sorted) << std::endl;
      D r = D(Word("A")) > D(Word("B")) > D(Word("C")) > D(Word("D")) > D(Word("E"));
      std::cout << r;
      std::cout << "right hanging: " << lambda(r) << std::endl;
      D reverse = D(Word("E")) < D(Word("D")) < D(Word("C")) < D(Word("B")) < D(Word("A"));
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


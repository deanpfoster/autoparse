//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "transition_probability.h"

class sample: public auto_parse::Transition_probability
{
public:
  ~sample(){};
  sample(){};

  void accumulate(const auto_parse::Word&, const auto_parse::Word&){};
  Transition_probability* renormalize() const{return 0;};

  sample* clone() const
  {
    return new sample(*this);
  };
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 double
  operator()(const auto_parse::Word& parent,const auto_parse::Word& child) const
  {
    if(parent < child)
      return 1.0;
    else
      return 0.0;
  };

  void
  print_on(std::ostream & ostrm) const
  {
    ostrm << "Heap: left is less than right." << std::endl;
  };
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




namespace auto_parse
{
  void test_transition_probability()
  {
    std::cout << "\n\n\n\t\t\t TRANSITION_PROBABILITY  TRANSITION_PROBABILITY  TRANSITION_PROBABILITY\n\n\n"<< std::endl;
    {
      sample t;  // testing construction
      std::cout << "constructed!" << std::endl;
      std::cout << t("A", "B") << " should be 2/3" << std::endl;
      std::cout << t("B", "A") << " should be 1/3" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_transition_probability();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


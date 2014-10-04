//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "transition_probability.h"

class sample: public auto_parse::Transition_probability
{
public:
  ~sample(){};
  sample(){};

  void accumulate(const auto_parse::Node&, const auto_parse::Node&, const auto_parse::Words&){};
  void accumulate(const auto_parse::Words&){};
  Transition_probability* renormalize() const{return 0;};

  sample* clone() const
  {
    return new sample(*this);
  };
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 double
 operator()(const auto_parse::Node& ,const auto_parse::Node& , const auto_parse::Words&) const
  {
    return 2./3.;
  };
double
 operator()(const auto_parse::Words&) const
  {
    return 2./3.;
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
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      Words words(&l);
      words.push_back(Word(l,"A"));
      words.push_back(Word(l,"B"));
      Words::const_iterator a = words.begin();
      Words::const_iterator b = words.begin();
      ++b;
      std::cout << t(a, b, words) << " should be 2/3" << std::endl;
      std::cout << t(b, a, words) << " should be 1/3" << std::endl;
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


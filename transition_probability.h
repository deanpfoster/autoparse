// -*- c++ -*-

#ifndef INCLUDED_TRANSITION_PROBABILITY
#define INCLUDED_TRANSITION_PROBABILITY

#include <iosfwd>
#include "word.h"

// Currently the model is:
// p=2/3 if Word1 < Word2
// p=1/3 if Word1 > Word2
// So it should look like a heap with high probability

namespace auto_parse
{
  class Transition_probability
  {
  public:
    // CONSTRUCTORS
    ~Transition_probability();
    Transition_probability();

    // MANIPULATORS
    // ACCESSORS
    virtual double operator()(const Word&,  const Word&) const;
    virtual void print_on(std::ostream &) const;

  protected:
  private:
    Transition_probability(const Transition_probability &);            // Don't delete this.
    Transition_probability& operator=(const Transition_probability &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Transition_probability &);

#endif

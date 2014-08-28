// -*- c++ -*-

#ifndef INCLUDED_TRANSITION_PROBABILITY
#define INCLUDED_TRANSITION_PROBABILITY

#include <iosfwd>
#include "word.h"

namespace auto_parse
{
  class Transition_probability
  {
  public:
    // CONSTRUCTORS
    virtual ~Transition_probability();
    Transition_probability();
    Transition_probability(const Transition_probability &);          
    virtual Transition_probability* clone() const = 0;

    // MANIPULATORS
    virtual void accumulate(const Word&, const Word&) = 0;
    virtual void merge(const Transition_probability&);
    virtual void renormalize() = 0;
    
    // ACCESSORS
    virtual double operator()(const Word&,  const Word&) const = 0;
    virtual void print_on(std::ostream &) const = 0;

  protected:
  private:
    Transition_probability& operator=(const Transition_probability &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Transition_probability &);

#endif

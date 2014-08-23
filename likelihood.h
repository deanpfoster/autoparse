// -*- c++ -*-

#ifndef INCLUDED_LIKELIHOOD
#define INCLUDED_LIKELIHOOD

#include <iosfwd>
#include "dependency.h"
#include "transition_probability.h"
#include <stack>

namespace auto_parse
{
  class Likelihood
  {
  public:
    // CONSTRUCTORS
    ~Likelihood();
    Likelihood(const Transition_probability& left, const Transition_probability& right); // read model from file
    Likelihood(const Likelihood &);          

    // MANIPULATORS
    // ACCESSORS
    double operator()(const Dependency&) const;
    void print_on(std::ostream & ostrm) const;
    
  private:
    Transition_probability* mp_left;
    Transition_probability* mp_right;
    
    Likelihood& operator=(const Likelihood &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Likelihood &);

#endif

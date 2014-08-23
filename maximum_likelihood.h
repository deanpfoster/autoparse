// -*- c++ -*-

#ifndef INCLUDED_MAXIMUM_LIKELIHOOD
#define INCLUDED_MAXIMUM_LIKELIHOOD

#include <iosfwd>
#include "dependency.h"
#include "transition_probability.h"
#include <stack>

namespace auto_parse
{
  class Likelihood;
  class Maximum_likelihood
  {
  public:
    // CONSTRUCTORS
    ~Maximum_likelihood();
    Maximum_likelihood(const Transition_probability& left,
		       const Transition_probability& right);

    // MANIPULATORS
    void operator()(const Dependency&);
    // ACCESSORS
    Likelihood output() const;
    
  private:
    Transition_probability* mp_left;
    Transition_probability* mp_right;

    Maximum_likelihood(); // read model from file
    Maximum_likelihood(const Maximum_likelihood &);          
    Maximum_likelihood& operator=(const Maximum_likelihood &); // Don't delete this. 
  };
}


#endif

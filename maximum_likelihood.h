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
  class Maximum_Likelihood
  {
  public:
    // CONSTRUCTORS
    ~Maximum_Likelihood();
    Maximum_Likelihood(const Transition_probability& left,
		       const Transition_probability& right);

    // MANIPULATORS
    void operator()(const Dependency&);
    // ACCESSORS
    Likelihood output() const;
    
  private:
    Transition_probability* mp_left;
    Transition_probability* mp_right;

    Maximum_Likelihood(); // read model from file
    Maximum_Likelihood(const Maximum_Likelihood &);          
    Maximum_Likelihood& operator=(const Maximum_Likelihood &); // Don't delete this. 
  };
}


#endif

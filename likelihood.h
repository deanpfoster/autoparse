// -*- c++ -*-

#ifndef INCLUDED_LIKELIHOOD
#define INCLUDED_LIKELIHOOD

#include <iosfwd>
#include "decorated_dependency.h"
#include "transition_probability.h"
#include <stack>

namespace auto_parse
{
  class Likelihood
  {
  public:
    // CONSTRUCTORS
    ~Likelihood();
    Likelihood(const Transition_probability& left_pointing, const Transition_probability& right_pointing, const Transition_probability& root); 
    Likelihood(const Likelihood &);
    Likelihood& operator=(const Likelihood &);

    // MANIPULATORS
    // ACCESSORS
    double operator()(const Dependency&) const;
    std::string summarize_pieces(const auto_parse::Vector&) const;
    auto_parse::Vector pieces(const Dependency&) const;
    Decorated_dependency decorate(const Dependency&, const Eigenwords&) const;
    void print_on(std::ostream & ostrm) const;
    double link_probability(const Link&, const Words&) const;
    
  private:
    Transition_probability* mp_left;
    Transition_probability* mp_right;
    Transition_probability* mp_root;
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Likelihood &);

#endif

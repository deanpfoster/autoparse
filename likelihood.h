// -*- c++ -*-

#ifndef INCLUDED_LIKELIHOOD
#define INCLUDED_LIKELIHOOD

#include <iosfwd>
#include "dependency.h"
#include <stack>

namespace auto_parse
{
  class Likelihood
  {
  public:
    // CONSTRUCTORS
    ~Likelihood();
    Likelihood(iostream&); // read model from file

    // MANIPULATORS
    // ACCESSORS
    double operator()(const Dependency&) const;

    
  private:
    Eigen_dictionary m_eigens; // These are also stored in the m_parse.  This duplication is wrong!
    Double
    std::stack<Node> m_stack;
    auto_parse::Dependency::const_word_iterator m_next_input;
    Dependency m_parse;
    
    LIKELIHOOD(const LIKELIHOOD &);            // Don't delete this.
    LIKELIHOOD& operator=(const LIKELIHOOD &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::LIKELIHOOD &);

#endif

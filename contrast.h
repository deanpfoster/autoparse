// -*- c++ -*-

#ifndef INCLUDED_CONTRAST
#define INCLUDED_CONTRAST

#include <iosfwd>
#include "statistical_parse.h"
#include "likelihood.h"
#include "feature_generator.h"

namespace auto_parse
{
  class Contrast
  {
  public:
    // CONSTRUCTORS
    ~Contrast();
    Contrast(const Statistical_parse&,
	     const Likelihood&,
	     const Feature_generator&);

    // MANIPULATORS
    // ACCESSORS
    void operator()(std::ostream&, const Words&) const;

  protected:

  private:
    Statistical_parse m_parser;
    Likelihood  m_likelihood;
    Feature_generator m_feature_generator;
    Contrast(const Contrast &);            // Don't delete this.
    Contrast& operator=(const Contrast &); // Don't delete this.
  };
}
#endif

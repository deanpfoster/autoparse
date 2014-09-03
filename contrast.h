// -*- c++ -*-

#ifndef INCLUDED_CONTRAST
#define INCLUDED_CONTRAST

#include <iosfwd>
#include "statistical_parse.h"
#include "likelihood.h"
#include "feature_generator.h"

namespace auto_parse
{
  class Row;
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
    std::vector<Row> operator()(const Words&) const;
    History suggest_alternative_history(const Words&, const auto_parse::History&) const;

  protected:
    History common(const History& h1,const History& h2) const;

  private:
    Statistical_parse m_parser;
    Likelihood  m_likelihood;
    Feature_generator m_feature_generator;
    mutable double m_count = 0;
    mutable double m_noise = 0;
    mutable double m_misses = 0;

    Contrast(const Contrast &);            // Don't delete this.
    Contrast& operator=(const Contrast &); // Don't delete this.
  };
}
#endif

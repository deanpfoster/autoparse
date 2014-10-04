// -*- c++ -*-

#ifndef INCLUDED_CONTRAST_HELPER
#define INCLUDED_CONTRAST_HELPER

#include <iosfwd>
#include "statistical_parse.h"
#include "feature_generator.h"

namespace auto_parse
{
  class Row;
  class Contrast_helper
  {
  public:
    // CONSTRUCTORS
    virtual ~Contrast_helper();
    Contrast_helper(const Statistical_parse&,
	     const Feature_generator&);

    // MANIPULATORS
    // ACCESSORS
    History suggest_alternative_history(const Words&, const auto_parse::History&) const;

  protected:
    History common(const History& h1,const History& h2) const;

  protected:
    Statistical_parse m_parser;
    Feature_generator m_feature_generator;

    Contrast_helper(const Contrast_helper &);            // Don't delete this.
    Contrast_helper& operator=(const Contrast_helper &); // Don't delete this.
  };
}
#endif

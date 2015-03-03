// -*- c++ -*-

#ifndef INCLUDED_CONTRAST
#define INCLUDED_CONTRAST

#include "contrast_helper.h"
#include "likelihood.h"

namespace auto_parse
{
  class Contrast: public Contrast_helper
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

  protected:

  private:
    Likelihood  m_likelihood;
    mutable double m_count = 0;
    mutable double m_noise = 0;
    mutable double m_misses = 0;

    Contrast(const Contrast &);            // Don't delete this.
    Contrast& operator=(const Contrast &); // Don't delete this.
  };
}
#endif

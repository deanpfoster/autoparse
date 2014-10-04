// -*- c++ -*-

#ifndef INCLUDED_GOLDEN_CONTRAST
#define INCLUDED_GOLDEN_CONTRAST

#include "contrast_helper.h"
#include "gold_standard.h"

namespace auto_parse
{
  class Row;
  class Golden_contrast: public Contrast_helper
  {
  public:
    // CONSTRUCTORS
    ~Golden_contrast();
    Golden_contrast(const Statistical_parse&,
		    const Feature_generator&);

    // ACCESSORS
    std::vector<Row> operator()(const Gold_standard&, const Words&) const;

  private:

    Golden_contrast(const Golden_contrast &);            // Don't delete this.
    Golden_contrast& operator=(const Golden_contrast &); // Don't delete this.
  };
}
#endif

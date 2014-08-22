// -*- c++ -*-

#ifndef INCLUDED_FEATURE_STACK_SIZE
#define INCLUDED_FEATURE_STACK_SIZE

#include "feature.h"

namespace auto_parse
{
  class Feature_stack_size: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Feature_stack_size();
    Feature_stack_size();

    // ACCESSORS
    data_iterator set_values(data_iterator, const LR&) const;  
    name_iterator set_names(name_iterator) const;
    std::string   name() const;
    int           dimension() const;

  private:
    Feature_stack_size(const Feature_stack_size &);            // Don't delete this.
    Feature_stack_size& operator=(const Feature_stack_size &); // Don't delete this.
  };
}

#endif

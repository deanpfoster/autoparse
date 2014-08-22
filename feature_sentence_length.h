// -*- c++ -*-

#ifndef INCLUDED_FEATURE_SENTENCE_LENGTH
#define INCLUDED_FEATURE_SENTENCE_LENGTH

#include "feature.h"

namespace auto_parse
{
  class Feature_sentence_length: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Feature_sentence_length();
    Feature_sentence_length();

    // ACCESSORS
    data_iterator set_values(data_iterator, const LR&) const;  
    name_iterator set_names(name_iterator) const;
    std::string   name() const;
    int           dimension() const;

  private:
    Feature_sentence_length(const Feature_sentence_length &);            // Don't delete this.
    Feature_sentence_length& operator=(const Feature_sentence_length &); // Don't delete this.
  };
}

#endif

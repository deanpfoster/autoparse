// -*- c++ -*-

#ifndef INCLUDED_FEATURE_WORDS_LEFT
#define INCLUDED_FEATURE_WORDS_LEFT

#include "feature.h"

namespace auto_parse
{
  class Feature_words_left: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Feature_words_left();
    Feature_words_left();
    Feature_words_left* clone() const;

    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR&) const;
    name_iterator set_names(name_iterator) const;
    std::string   name() const;
    int           dimension() const;

  private:
    Feature_words_left(const Feature_words_left &);            // Don't delete this.
    Feature_words_left& operator=(const Feature_words_left &); // Don't delete this.
  };
}

#endif

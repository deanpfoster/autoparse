// -*- c++ -*-

#ifndef INCLUDED_FEATURE_WORDS_LEFT
#define INCLUDED_FEATURE_WORDS_LEFT

#include "feature.h"
#include "lr.h"

namespace auto_parse
{

  struct Words_left
  {
    double operator()(const LR& parser) const { return parser.number_words_left();}
    std::string variable_name() const{return "number_words_left";};
    std::string descriptive_name() const{return "number of words left in the sentence.";};
  };

  struct Stack_size
  {
    double operator()(const LR& parser) const { return parser.stack_size();}
    std::string variable_name() const{return "stack_size";};
    std::string descriptive_name() const{return "Size of the current stack.";};
  };

  struct Sentence_length
  {
    double operator()(const LR& parser) const
    {
      const Words& sentence = parser.parse().sentence();
      return sentence.end() - sentence.begin();
    }
    std::string variable_name() const{return "sentence_length";};
    std::string descriptive_name() const{return "Full sentence length.";};
  };



  template <class T>
  class Feature_one_dimensional: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Feature_one_dimensional<T>();
    Feature_one_dimensional<T>();
    Feature_one_dimensional<T>* clone() const;

    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR&) const;
    name_iterator set_names(name_iterator) const;
    std::string   name() const;
    int           dimension() const;

  private:
    Feature_one_dimensional<T>(const Feature_one_dimensional<T> &);            // Don't delete this.
    Feature_one_dimensional<T>& operator=(const Feature_one_dimensional<T> &); // Don't delete this.
  };
}

#endif

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

  template<int i>
  struct Distance_to
  {
    double operator()(const LR& parser) const
    {
      if(parser.stack_size() <= i)
	return 0; // we don't have a stack of sufficient depth
      const Words& sentence = parser.parse().sentence();
      const Node& location = parser.stack(i);
      return location - sentence.begin();
    }
    std::string variable_name() const
    {
      std::stringstream s;
      s << "distance_to_" << i;
      return s.str();
    };
    std::string descriptive_name() const
    {
      std::stringstream s;
      s << "Distance to stack location " << i << ".";
      return s.str();
    }
  };

  template<class T>
  struct Squared
  {
    double operator()(const LR& parser) const
    {
      double tmp = T()(parser);
      return tmp * tmp;
    }
    std::string variable_name() const
    {
      std::stringstream s;
      s << T().variable_name() << "_squared";
      return s.str();
    };
    std::string descriptive_name() const
    {
      std::stringstream s;
      s << "The square of `" << T().descriptive_name() << "'.";
      return s.str();
    }
  };
  
  template<class T1, class T2>
  struct Interaction
  {
    double operator()(const LR& parser) const
    {
      double tmp1 = T1()(parser);
      double tmp2 = T2()(parser);
      return tmp1 * tmp2;
    }
    std::string variable_name() const
    {
      std::stringstream s;
      s << T1().variable_name() << "_x_" << T2().variable_name();
      return s.str();
    };
    std::string descriptive_name() const
    {
      std::stringstream s;
      s << "`" << T1().descriptive_name()
	<< "' times `" << T2().descriptive_name() << "'.";
      return s.str();
    }
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

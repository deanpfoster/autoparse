// -*- c++ -*-

#ifndef INCLUDED_FEATURE_EIGENWORDS
#define INCLUDED_FEATURE_EIGENWORDS

#include "feature.h"
#include "word.h"
#include "lr.h"
#include "eigenwords.h"

namespace auto_parse
{
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Next_word
  {
    Word operator()(const LR& parser) const
    {
      if(parser.number_words_left() == 0)
	return Word();
      return *parser.next_word();
    }
    std::string name() const{return "next_word";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Next_word_1  // zero based indexing.  :-)
  {
    Word operator()(const LR& parser) const
    {
      if(parser.number_words_left() > 1)
	return *parser.next_next_word();
      else
	return "";
    }
    std::string name() const{return "next_word";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Stack_top
  {
    Word operator()(const LR& parser) const
    {
      if(parser.stack_size() > 0)
	return *(parser.stack_top());
      else
	return Word();
    }
    std::string name() const{return "stack_top";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Stack_top_left_most
  {
    Word operator()(const LR& parser) const
    {
      if(parser.stack_size() == 0)
	return Word();
      Node t = parser.stack_top();
      assert(t != parser.parse().sentence().end());
      Node child = parser.parse().left_most_child(t);
      if(child == parser.parse().sentence().end())
	return Word();
      return *child;
	return Word();
    }
    std::string name() const{return "stack_top_left_most";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Stack_top_right_most
  {
    Word operator()(const LR& parser) const
    {
      if(parser.stack_size() == 0)
	return Word();
      Node t = parser.stack_top();
      assert(t != parser.parse().sentence().end());
      Node child = parser.parse().right_most_child(t);
      if(child == parser.parse().sentence().end())
	return Word();
      return *child;
    }
    std::string name() const{return "stack_top_right_most";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Next_word_right_most
  {
    Word operator()(const LR& parser) const
    {
      if(parser.number_words_left() == 0)
	return Word();
      Node t = parser.next_word();
      assert(t != parser.parse().sentence().end());
      Node child = parser.parse().right_most_child(t);
      if(child == parser.parse().sentence().end())
	return Word();
      return *child;
    }
    std::string name() const{return "next_word_right_most";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Next_word_left_most
  {
    Word operator()(const LR& parser) const
    {
      if(parser.number_words_left() == 0)
	return Word();
      Node t = parser.next_word();
      assert(t != parser.parse().sentence().end());
      Node child = parser.parse().left_most_child(t);
      if(child == parser.parse().sentence().end())
	return Word();
      return *child;
    }
    std::string name() const{return "next_word_left_most";};
  };
  /////////////////////////////////////////////////////////////////////////////////////////
  struct Stack_1
  {
    Word operator()(const LR& parser) const{
      if(parser.stack_size() <= 1)
	return Word();
      return *(parser.stack(1));
    }
    std::string name() const{return "stack_1";};
  };
  struct Stack_2
  {
    Word operator()(const LR& parser) const{return *(parser.stack(2));}
    std::string name() const{return "stack_2";};
  };


  // next_input(), *stack(1), 
    

  template<class T>
  class Feature_eigenwords: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Feature_eigenwords();
    Feature_eigenwords(const Eigenwords&);
    Feature_eigenwords(const Feature_eigenwords&);
    Feature_eigenwords(std::istream&);
    Feature_eigenwords* clone() const;
    // ACCESSORS
    Eigen::VectorXd operator()(const LR&) const;  
    std::vector<std::string> variable_names() const;
    std::string   name() const;
    int           dimension() const;

  private:
    Eigenwords m_eigenwords;
    Feature_eigenwords& operator=(const Feature_eigenwords &); // Don't delete this.
    Feature_eigenwords* private_restore(std::istream&) const;
  };
}

#endif

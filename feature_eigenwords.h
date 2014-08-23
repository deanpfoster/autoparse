// -*- c++ -*-

#ifndef INCLUDED_FEATURE_EIGENWORDS
#define INCLUDED_FEATURE_EIGENWORDS

#include "feature.h"
#include "word.h"
#include "lr.h"
#include "eigenwords.h"

namespace auto_parse
{
  struct Next_word
  {
    Word operator()(const LR& parser) const{return parser.next_word();}
    std::string name() const{return "next_word";};
  };
  struct Stack_top
  {
    Word operator()(const LR& parser) const{return *(parser.stack_top());}
    std::string name() const{return "stack_top";};
  };
  struct Stack_1
  {
    Word operator()(const LR& parser) const{return *(parser.stack(1));}
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
    Feature_eigenwords* clone() const;
    // ACCESSORS
    Eigen::VectorXd operator()(const LR&) const;  
    name_iterator set_names(name_iterator) const;
    std::string   name() const;
    int           dimension() const;

  private:
    Eigenwords m_eigenwords;
    Feature_eigenwords& operator=(const Feature_eigenwords &); // Don't delete this.
  };
}

#endif

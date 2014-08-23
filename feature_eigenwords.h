// -*- c++ -*-

#ifndef INCLUDED_FEATURE_EIGENWORDS
#define INCLUDED_FEATURE_EIGENWORDS

#include "feature.h"
#include "word.h"
#include "lr.h"
#include "eigenwords.h"

namespace auto_parse
{
  struct Stack_top
  {
    Word operator()(const LR& parser) const{return *(parser.stack_top());}
    std::string name() const{return "stack_top";};
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

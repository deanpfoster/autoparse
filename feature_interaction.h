// -*- c++ -*-

#ifndef INCLUDED_INTERACTION
#define INCLUDED_INTERACTION

#include "feature.h"
#include "lr.h"

namespace auto_parse
{
  template<class T1, class T2>
  class Interaction: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Interaction();
    Interaction(const T1& x1, const T2& x2);
    Interaction<T1,T2>* clone() const;


    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR& parser) const;
    std::vector<std::string> variable_names() const;
    std::string   name() const;
    int dimension() const;

  private:
    T1 m_x1;
    T2 m_x2;
    int m_dimension;
    Interaction<T1,T2>(const Interaction<T1,T2> &);            // Don't delete this.
    Interaction<T1,T2>& operator=(const Interaction<T1,T2> &); // Don't delete this.
  };
}

#endif

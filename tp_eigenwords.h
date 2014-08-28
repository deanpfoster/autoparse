// -*- c++ -*-

#ifndef INCLUDED_TP_EIGENWORDS
#define INCLUDED_TP_EIGENWORDS

#include <iosfwd>
#include "word.h"
#include "transition_probability.h"
#include "eigenwords.h"
#include <Eigen/Core>

namespace auto_parse
{
  class TP_eigenwords: public Transition_probability
  {
  public:
    // CONSTRUCTORS
    ~TP_eigenwords();
    TP_eigenwords(const Eigenwords& parent,const Eigenwords& child,
		  const Eigen::MatrixXd&);
    TP_eigenwords(const Eigenwords& parent, const Eigenwords& child);
    TP_eigenwords(const TP_eigenwords &);          
    virtual TP_eigenwords* clone() const;

    // MANIPULATORS
    virtual void accumulate(const Word& parent, const Word& child);
    virtual void merge(const Transition_probability&);
    virtual void renormalize();
    
    // ACCESSORS
    virtual double operator()(const Word& parent,  const Word& child) const;
    virtual void print_on(std::ostream &) const;

  protected:
  private:
    Eigenwords m_parent;
    Eigenwords m_child;
    Eigen::MatrixXd m_matrix;

    TP_eigenwords& operator=(const TP_eigenwords &); // Don't delete this.
  };
}

#endif

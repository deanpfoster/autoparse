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
    TP_eigenwords(const Eigenwords&,
		  const Eigen::MatrixXd&);
    TP_eigenwords(const TP_eigenwords &);          

    // MANIPULATORS
    // ACCESSORS
    virtual double operator()(const Word&,  const Word&) const;
    virtual void print_on(std::ostream &) const;

  protected:
  private:
    Eigenwords m_eigenwords;
    Eigen::MatrixXd m_matrix;

    TP_eigenwords& operator=(const TP_eigenwords &); // Don't delete this.
  };
}

#endif

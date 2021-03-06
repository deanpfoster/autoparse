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
		  const Eigen::MatrixXd& Parent_x_Parent, const Eigen::MatrixXd& Parent_x_Child, double scaling,
		  const std::vector<double>& distances, const std::vector<double>& average_number_children);
    TP_eigenwords(const Eigenwords& parent, const Eigenwords& child, double scaling);
    TP_eigenwords(const TP_eigenwords &);          
    virtual TP_eigenwords* clone() const;

    // MANIPULATORS
    virtual void accumulate(const Node& parent, const Node& child, const Words&);
    virtual void accumulate(const Words&);
    virtual void merge(const Transition_probability&);
    
    // ACCESSORS
    virtual double operator()(const Node& parent,  const Node& child, const Words&) const;
    virtual double operator()(const Words&) const;
    virtual void print_on(std::ostream &) const;
    virtual TP_eigenwords* renormalize() const;

  protected:
  private:
    Eigenwords m_parent;
    Eigenwords m_child;
    Eigen::MatrixXd m_XtY;
    Eigen::MatrixXd m_XtX;
    double m_scaling;  // arbitary tradeoff between distance and fit, eventually should be a parameter
    std::vector<double> m_distance;
    std::vector<double> m_count;  // how often a parent occurs
    std::vector<double> m_total; // total number of children

    TP_eigenwords& operator=(const TP_eigenwords &); // Don't delete this.
  };
}

#endif

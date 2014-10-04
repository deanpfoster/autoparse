// -*- c++ -*-

#ifndef INCLUDED_TP_IID
#define INCLUDED_TP_IID

#include <iosfwd>
#include "word.h"
#include "transition_probability.h"

namespace auto_parse
{
  class TP_iid: public Transition_probability
  {
  public:
    // CONSTRUCTORS
    ~TP_iid();
    TP_iid(const std::vector<double>& average_number_children, double scaling);
    TP_iid(int size, double scaling);
    TP_iid(const TP_iid &);          
    virtual TP_iid* clone() const;

    // MANIPULATORS
    virtual void accumulate(const Node& parent, const Node& child, const Words&);
    virtual void accumulate(const Words&);
    virtual void merge(const Transition_probability&);
    
    // ACCESSORS
    virtual double operator()(const Node& parent,  const Node& child, const Words&) const;
    virtual double operator()(const Words&) const;
    virtual void print_on(std::ostream &) const;
    virtual TP_iid* renormalize() const;

  protected:
  private:
    double m_scaling;
    std::vector<double> m_total; // total number of children

    TP_iid& operator=(const TP_iid &); // Don't delete this.
  };
}

#endif

// -*- c++ -*-

#ifndef INCLUDED_INTERACTION
#define INCLUDED_INTERACTION

#include "feature.h"
#include "lr.h"

namespace auto_parse
{
  class Interaction: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Interaction();
    Interaction(const Feature& x1, const Feature& x2);
    Interaction(std::istream&);
    Interaction(const Interaction &);
    Interaction* clone() const;
    Interaction* private_restore(std::istream&) const;


    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR& parser) const;
    std::vector<std::string> variable_names() const;
    std::string   name() const;
    int dimension() const;

  private:
    Feature* mp_x1;
    Feature* mp_x2;
    int m_dimension;
    Interaction& operator=(const Interaction &); // Don't delete this.
  };

  inline
  Interaction
  operator*(const Feature& x1, const Feature& x2)
  {
    return Interaction(x1,x2);
  }

}

#endif

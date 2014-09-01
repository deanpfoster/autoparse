// -*- c++ -*-

#ifndef INCLUDED_SHORTEN
#define INCLUDED_SHORTEN

#include "feature.h"
#include "lr.h"

namespace auto_parse
{
  class Shorten: public Feature
  {
  public:
    // CONSTRUCTORS
    virtual ~Shorten();
    Shorten(const Feature& x1, int length);
    Shorten* clone() const;


    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR& parser) const;
    std::vector<std::string> variable_names() const;
    std::string   name() const;
    int dimension() const;

  private:
    Feature* mp_long;
    int m_dimension;
    Shorten(const Shorten &);            // Don't delete this.
    Shorten& operator=(const Shorten &); // Don't delete this.
  };
}

#endif

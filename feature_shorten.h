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
    Shorten(const Shorten &);
    Shorten(std::istream &);
    Shorten* clone() const;


    // ACCESSORS
    virtual auto_parse::Vector operator()(const LR& parser) const;
    std::vector<std::string> variable_names() const;
    std::string   name() const;
    int dimension() const;

  private:
    Feature* mp_long;
    int m_dimension;
    Shorten& operator=(const Shorten &); // Don't delete this.
    Shorten* private_restore(std::istream&) const;
  };

  // helper
  inline Shorten shorten(const Feature& x1, int length)
  {
    return Shorten(x1,length);
  }
}

#endif

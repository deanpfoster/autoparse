// -*- c++ -*-

#ifndef INCLUDED_MODEL
#define INCLUDED_MODEL

#include <iosfwd>
#include <map>
#include "history.h"

namespace auto_parse
{
  class Model
  {
  public:
    // CONSTRUCTORS
    ~Model();
    Model(std::istream&);

    // MANIPULATORS
    // ACCESSORS
    Delta_forecasts operator()(const LR&) const;

    virtual void print_on(std::ostream &) const;

  protected:
    std::map<Action, Forecast> m_forecasts;

  private:
    Model(const Model &);            // Don't delete this.
    Model& operator=(const Model &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Model &);

#endif

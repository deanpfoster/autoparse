// -*- c++ -*-

#ifndef INCLUDED_MODEL
#define INCLUDED_MODEL

#include <iosfwd>
#include <map>
#include "history.h"

namespace auto_parse
{
  class Forecast;
  class LR;
  class Value_of_forecasts;
  
  class Model
  {
  public:
    // CONSTRUCTORS
    ~Model();
    Model(std::istream&);
    Model(const Model &);
    Model(); // creates an empty and broken model.  Use add_forecast to repair
    void add_forecast(Action, const Forecast*);

    // MANIPULATORS
    // ACCESSORS
    Value_of_forecasts operator()(const LR&) const;

    void print_on(std::ostream &) const;
    void save(std::ostream & ) const;

  protected:
    std::map<Action, const Forecast*> m_forecasts;

  private:
    Model& operator=(const Model &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Model &);

#endif

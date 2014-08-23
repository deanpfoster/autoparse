// -*- c++ -*-

#ifndef INCLUDED_MODEL
#define INCLUDED_MODEL

#include <iosfwd>
#include <map>
#include "history.h"
#include "feature_generator.h"

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
    Model(std::istream&,const Feature_generator&);
    Model(const Model &);
    Model(); // creates an empty and broken model.  Use add_forecast to repair
    void add_forecast(Action, const Forecast*);
    Model(const std::initializer_list<std::pair<auto_parse::Action,Forecast*> >&, const Feature_generator&);
    

    // MANIPULATORS
    // ACCESSORS
    Value_of_forecasts operator()(const LR&) const;

    void print_on(std::ostream &) const;
    void save(std::ostream & ) const;

  protected:
    std::map<Action, const Forecast*> m_forecasts;
    Feature_generator m_features;

  private:
    Model& operator=(const Model &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Model &);

#endif

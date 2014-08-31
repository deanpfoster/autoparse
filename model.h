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
    Model(const Feature_generator&);  // creates an empty and broken model.  Use add_forecast to repair
    void add_forecast(Action, const Forecast&);
    void tweak_forecast(Action, const Forecast&, double fraction);
    void tweak(const Model&, double fraction);
    Model(const std::initializer_list<std::pair<auto_parse::Action,Forecast*> >&, const Feature_generator&);
    Model& operator=(const Model &); 
    

    // MANIPULATORS
    // ACCESSORS
    Value_of_forecasts operator()(const LR&) const;
    const Forecast& forecast(Action a) const{return *(m_forecasts.find(a)->second);};

    void print_on(std::ostream &) const;
    void save(std::ostream & ) const;

  protected:
    std::map<Action, Forecast*> m_forecasts;
    Feature_generator m_features;

  private:
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Model &);

#endif

// -*- c++ -*-

#ifndef INCLUDED_MODEL
#define INCLUDED_MODEL

#include <iosfwd>
#include <map>
#include "history.h"
#include <Eigen/Core>
#include "vector.h"

namespace auto_parse
{
  class Forecast;
  class Value_of_forecasts;
  
  class Model
  {
  public:
    // CONSTRUCTORS
    ~Model();
    Model(std::istream&);
    Model(const Model &);
    Model();  // generates a broken Model.  Use add_forecast afterwards.
    void add_forecast(Action, const Forecast&);
    void tweak_forecast(Action, const Forecast&, double fraction);
    void tweak(const Model&, double fraction);
    Model(const std::initializer_list<std::pair<auto_parse::Action,Forecast*> >&);
    Model& operator=(const Model &); 
    

    // MANIPULATORS
    // ACCESSORS
    Value_of_forecasts operator()(const auto_parse::Vector& features) const;
    const Forecast& forecast(Action a) const{return *(m_forecasts.find(a)->second);};
    const std::vector<Action>& all_actions() const{return m_all_actions;};

    void print_on(std::ostream &) const;
    void save(std::ostream & ) const;

  protected:
    std::map<Action, Forecast*> m_forecasts;
    std::vector<Action> m_all_actions;

  private:
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Model &);

#endif

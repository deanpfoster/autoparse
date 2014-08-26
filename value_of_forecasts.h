// -*- c++ -*-

#ifndef INCLUDED_VALUE_OF_FORECASTS
#define INCLUDED_VALUE_OF_FORECASTS

#include <iosfwd>
#include <map>
#include "history.h"

namespace auto_parse
{
  class Value_of_forecasts
  {
  public:
    ~Value_of_forecasts();
    Value_of_forecasts();
    Value_of_forecasts(const std::map<Action,double>&);
    // modifiers
    double& operator[](Action);
    void zero_second_best();
    // ACCESSORS
    Action best_action() const;
    double best_value() const;
    Action alternative_action() const;
  private:
    std::map<Action, double> m_values;
  };
  
}


#endif

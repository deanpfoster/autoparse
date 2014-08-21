// -*- c++ -*-

#ifndef INCLUDED_STATISTICAL_HISTORY
#define INCLUDED_STATISTICAL_HISTORY

#include "history.h"
#include "value_of_forecasts.h"

namespace auto_parse
{
  class Statistical_history: public History
  {
  public:
    // CONSTRUCTORS
    ~Statistical_history();
    Statistical_history();
    Statistical_history(const Statistical_history &);

    // MANIPULATORS
    void push_back(Action, const Value_of_forecasts&);
    // ACCESSORS
    const std::vector<Value_of_forecasts>& values() const;
    
  protected:

  private:
    std::vector<Value_of_forecasts> m_values;
  };
}

#endif

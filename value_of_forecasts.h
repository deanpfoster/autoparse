// -*- c++ -*-

#ifndef INCLUDED_VALUE_OF_FORECASTS
#define INCLUDED_VALUE_OF_FORECASTS

#include <iosfwd>
#include <map>
#include "history.h"

namespace auto_parse
{
  class Value_of_forecasts: public std::map<Action, double>
  {
  };
  
}


#endif

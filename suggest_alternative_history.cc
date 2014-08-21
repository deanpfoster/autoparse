// -*- c++ -*-

#include "suggest_alternative_history.h"
#include "statistical_history.h"
#include "assert.h"
#include "utilities/z.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

auto_parse::History
auto_parse::suggest_alternative_history(const auto_parse::Statistical_history& h)
{
  double arbitary_constant = 1.0;
  History result;
  auto v = h.values();
  double total = 0;
  for(Value_of_forecasts i : v)
    assert(i.best_value() >= 0);
  for(Value_of_forecasts i : v)
    total += 1/(arbitary_constant + i.best_value());
  double uniform = my_random::U();
  double cum_sum = 0;
  auto i = v.begin();
  auto j = h.begin();
  while(cum_sum < uniform)
    {
      cum_sum += 1/(arbitary_constant + i->best_value())/total;
      result.push_back(*j);
      ++i;
      ++j;
    }
  assert(i != v.end());
  assert(i - v.begin() == j - h.begin());
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

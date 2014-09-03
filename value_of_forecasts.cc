// -*- c++ -*-


#include "value_of_forecasts.h"

// put other includes here
#include "assert.h"
#include <math.h>
#include "utilities/z.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Value_of_forecasts::~Value_of_forecasts()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Value_of_forecasts::Value_of_forecasts()
  :m_values()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Value_of_forecasts::Value_of_forecasts(const std::map<Action, double> & m)
  :m_values(m)
{
};

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double&
auto_parse::Value_of_forecasts::operator[](Action a)
{
  return m_values[a];
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Value_of_forecasts::zero_second_best()
{
  double max = m_values[Action::shift];
  Action arg_max = Action::shift;
  for(Action a: auto_parse::all_actions)
    {
      assert(!isnan(m_values[a]));
    if(m_values[a] > max)
      {
	max = m_values[a];
	arg_max = a;
      }
    }
  double second_best = -1e10;
  for(Action a: all_actions)
    if(m_values[a] > second_best)
      if(a != arg_max)
	{
	  second_best = m_values[a];
	}
  if(second_best > -1e9)
    for(Action a: all_actions)
      m_values[a] = m_values[a] - second_best;
  else
    for(Action a: all_actions)
      m_values[a] = m_values[a] - max;
}

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Action
auto_parse::Value_of_forecasts::best_action() const
{
  Action result = Action::head_reduce;
  double max = m_values.find(result)->second;
  for (Action a : all_actions)
    {
      double value = m_values.find(a)->second;
      if(value > max)
	{
	  max = value;
	  result = a;
	}
    }
  assert(max < 100);
  return result;
};

auto_parse::Action
auto_parse::Value_of_forecasts::smoothed_best_action(double noise) const
{
  if(noise == 0)
    return best_action();

  double total = 0;
  for (Action a : all_actions)
    {
      double value = m_values.find(a)->second;
      assert(value < 100);
      if(value > -100)
	total += exp(value / noise);
    }
  double which = total * my_random::U();
  for (Action a : all_actions)
    { // sneaky use of total.  This is bad style!  But I'm too drunk to write it with the extra variable.
      double value = m_values.find(a)->second;
      which -= exp(value / noise);
      if(which <= 0)
	return a;
    }; // at least I didn't write "if(total -= which <= 0) return a;" which would have saved 5 lines!
  assert(0);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


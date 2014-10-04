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
  assert(m_values.size() > 0);
  std::pair<Action,double> best = *m_values.begin();
  for(std::pair<Action,double> p: m_values)
    if(p.second > best.second)
      best = p;
  
  double second_best = -1e10;
  for(std::pair<Action,double> p: m_values)
    if(p.second > second_best)
      if(p != best)
	second_best = p.second;

  if(second_best > -1e9)
    for(std::pair<Action,double> p: m_values)
      p.second = p.second - second_best;
  else
    for(std::pair<Action,double> p: m_values)
      p.second = p.second - best.second;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Action
auto_parse::Value_of_forecasts::best_action() const
{
  assert(m_values.size() > 0);
  std::pair<Action,double> best = *m_values.begin();
  for(std::pair<Action,double> p: m_values)
    if(p.second > best.second)
      best = p;
  return best.first;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Action
auto_parse::Value_of_forecasts::smoothed_best_action(double noise) const
{
  if(noise == 0)
    return best_action();

  double total = 0;
  for(std::pair<Action,double> p: m_values)
    {
      double value = p.second/noise;
      if(value > 100)
	return p.first;
      if(value > -100)
	total += exp(value);
    }
  assert(total != 0);
  double which = total * my_random::U_thread_safe();
  double cumsum = 0;
  for(std::pair<Action,double> p: m_values)
    {
      double value = p.second/noise;
      if(value > -100)
	{
	  cumsum += exp(value);
	  if(cumsum >= which)
	    return p.first;
	}
    };
  assert(0);
  return(best_action());
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


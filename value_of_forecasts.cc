// -*- c++ -*-


#include "value_of_forecasts.h"

// put other includes here
#include "assert.h"

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
    if(m_values[a] > max)
      {
	max = m_values[a];
	arg_max = a;
      }
  double second_best = -1e10;
  for(Action a: all_actions)
    if(m_values[a] > second_best)
      if(a != arg_max)
	{
	  second_best = m_values[a];
	}
  for(Action a: all_actions)
    m_values[a] = m_values[a] - second_best;
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
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double
auto_parse::Value_of_forecasts::best_value() const
{
  return( m_values.find(best_action())->second);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Action
auto_parse::Value_of_forecasts::alternative_action() const
{
  Action best = best_action();
  Action result = Action::head_reduce; 
  double max = m_values.find(result)->second;
  for (Action a : all_actions)
    {
      if(a != best)
	{
	  double value = m_values.find(a)->second;
	  if(value > max)
	    {
	      max = value;
	      result = a;
	    }
	}
    }
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

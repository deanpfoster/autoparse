// -*- c++ -*-


#include "value_of_forecasts.h"

// put other includes here
#include "assert.h"


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Action
auto_parse::Value_of_forecasts::best_action() const
{
  Action result = Action::head_reduce; 
  double max = this->find(result)->second;
  for (Action a : all_actions)
    {
      double value = this->find(a)->second;
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
  return( find(best_action())->second);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Action
auto_parse::Value_of_forecasts::alternative_action() const
{
  Action best = best_action();
  Action result = Action::head_reduce; 
  double max = this->find(result)->second;
  for (Action a : all_actions)
    {
      if(a != best)
	{
	  double value = this->find(a)->second;
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

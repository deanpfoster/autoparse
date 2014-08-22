// -*- c++ -*-


#include "model.h"
#include "forecast.h"
#include "value_of_forecasts.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <set>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Model::~Model()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(std::istream& in)
  : m_forecasts()  
{
  for(Action a: all_actions)
    {
      Action check;
      in >> check >> std::ws;
      assert(check == a);
      Forecast* p_forecast;
      in >> p_forecast;
      m_forecasts[a] = p_forecast;
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(const auto_parse::Model & other)
  :
  m_forecasts(other.m_forecasts)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model()
  :
  m_forecasts()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(const std::initializer_list<std::pair<auto_parse::Action,Forecast*> >& args)
  :
  m_forecasts()
{
  std::set<auto_parse::Action> check;
  for(std::pair<auto_parse::Action,Forecast*> p : args)
    {
      m_forecasts[p.first] = p.second;
      check.insert(p.first);
    }
  assert(check.size() == all_actions.size()); // there are better tests, but this will catch some errors
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Model::add_forecast(Action a, const Forecast* p_f)
{
  m_forecasts[a] = p_f;
}
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Value_of_forecasts
auto_parse::Model::operator()(const LR& parser) const
{
  Value_of_forecasts result;
  for(Action a: all_actions)
    result[a] = (*m_forecasts.find(a)->second)(parser);
  double max = result[Action::shift];
  Action arg_max = Action::shift;
  for(Action a: auto_parse::all_actions)
    if(result[a] > max)
      {
	max = result[a];
	arg_max = a;
      }
  double second_best = -1e10;
  for(Action a: all_actions)
    if(result[a] > second_best)
      if(a != arg_max)
	{
	  second_best = result[a];
	}
  for(Action a: all_actions)
    result[a] = result[a] - second_best;
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Model::print_on(std::ostream & ostrm) const
{
  for(Action a : all_actions)
    ostrm << "Forecast " << a << ":  " << *m_forecasts.find(a)->second;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Model::save(std::ostream & ostrm) const
{
  for(Action a : all_actions)
    {
      ostrm << a << std::endl;
      ostrm << *m_forecasts.find(a)->second;
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Model & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

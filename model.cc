// -*- c++ -*-


#include "model.h"
#include "forecast.h"
#include "value_of_forecasts.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <set>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Model::~Model()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(std::istream& in,const Feature_generator& features)
  : m_forecasts(),
    m_features(features)
{
  for(Action a: all_actions)
    {
      Action check;
      in >> check >> std::ws;
      assert(check == a);
      Forecast* p_forecast;
      in >> p_forecast;
      m_forecasts[a] = p_forecast->clone();
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(const auto_parse::Model & other)
  :
  m_forecasts(other.m_forecasts),
  m_features(other.m_features)
{
    for(Action a: all_actions)
    {
      m_forecasts[a] = other.m_forecasts.find(a)->second->clone();
    }
      
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(const auto_parse::Feature_generator& gen)
  :
  m_forecasts(),
  m_features(gen)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(const std::initializer_list<std::pair<auto_parse::Action,Forecast*> >& args, const auto_parse::Feature_generator& gen)
  :
  m_forecasts(),
  m_features(gen)
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
auto_parse::Model::add_forecast(Action a, const Forecast& f)
{
  m_forecasts[a] = f.clone();
}
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
auto_parse::Model&
auto_parse::Model::operator=(const auto_parse::Model& rhs)
{
  //  for(Action a : all_actions)
  //    delete m_forecasts[a];
  for(Action a : all_actions)
    m_forecasts[a] = rhs.m_forecasts.find(a)->second->clone();
  return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Value_of_forecasts
auto_parse::Model::operator()(const LR& parser) const
{
  Value_of_forecasts result;
  for(Action a: all_actions)
    {
      double value = (*m_forecasts.find(a)->second)(m_features(parser));
      assert(!isnan(value));
      if(value < -1e10)
	value = -1e10;
      if(value > 100)  // no large positive forecasts make sense
	value = 100;
      result[a] = value;
    }
  result.zero_second_best();
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

// -*- c++ -*-


#include "model.h"

// put other includes here
#include "assert.h"
#include <iostream>

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
    m_forecasts[a] = Forecast(in);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Model::Model(const Model & other)
  :
  m_forecasts(other.m_forecasts)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Delta_forecast
auto_parse::Model::operator()(const LR& parser) const
{
  Delta_forecast result;
  for(Action a: all_actions)
    result[a] = m_forecasts[a](parser);
  double max = result[Action::shift];
  double arg_max = Action::shift
  for(Action a: all_actions)
    if(result[a] > max)
      {
	max = result[a];
	arg_max = a;
      }
  double second_best = -1e10;
  double arg_second_best = Action::shift;
  for(Action a: all_actions)
    if(result[a] > second_best)
      if(a != arg_max)
	{
	  second_best = result[a];
	  arg_second_best = a;
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
    ostrm << "Forecast " << a << ":  " << m_forecasts[a];
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

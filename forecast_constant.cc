// -*- c++ -*-


#include "forecast_constant.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Forecast_constant::~Forecast_constant()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_constant::Forecast_constant()
  :Forecast(),
   m_constant()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_constant::Forecast_constant(const Forecast_constant & other) 
  :   Forecast(),
      m_constant(other.m_constant)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_constant::Forecast_constant(std::istream & in) 
  : Forecast(),
    m_constant()
{
  in >> m_constant >> std::ws;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_constant::Forecast_constant(double d) 
  : Forecast(),
    m_constant(d)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Forecast_constant::save(std::ostream & out) const
{
  out << m_constant << std::endl;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// This is basically the name of the class as a virtual function
std::string
auto_parse::Forecast_constant::key() const
{
  return "auto_parse::Forecast_constant";
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast*
auto_parse::Forecast_constant::restore(std::istream& in) const
{
  return new auto_parse::Forecast_constant(in);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

double
auto_parse::Forecast_constant::operator()(const Eigen::VectorXd&) const
{
  return m_constant;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

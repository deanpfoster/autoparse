// -*- c++ -*-


#include "forecast_linear.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Forecast_linear::~Forecast_linear()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear::Forecast_linear()
  :Forecast(),
   m_weights()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear::Forecast_linear(const Forecast_linear & other) 
  :   Forecast(),
      m_weights(other.m_weights)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear::Forecast_linear(std::istream & in) 
  : Forecast(),
    m_weights()
{
  int number;
  in >> number;
  m_weights = Eigen::VectorXd(number);
  for(int i = 0; i < number; ++i)
    {
      double d;
      in >> d >> std::ws;
      m_weights[i] = d;
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear::Forecast_linear(const Eigen::VectorXd& d) 
  : Forecast(),
    m_weights(d)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Forecast_linear::save(std::ostream & out) const
{
  out << m_weights.size() << std::endl;
  for(int i = 0; i < m_weights.size(); ++i)
    out << m_weights[i] <<  " ";
  out << std::endl;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// This is basically the name of the class as a virtual function
std::string
auto_parse::Forecast_linear::key() const
{
  return "auto_parse::Forecast_linear";
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast*
auto_parse::Forecast_linear::restore(std::istream& in) const
{
  return new auto_parse::Forecast_linear(in);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear*
auto_parse::Forecast_linear::clone() const
{
  return new Forecast_linear(*this);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

double
auto_parse::Forecast_linear::operator()(const Eigen::VectorXd& row) const
{
  return m_weights.transpose() * row;
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

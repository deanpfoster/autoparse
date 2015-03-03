// -*- c++ -*-


#include "forecast_linear.h"
#include "assert.h"
#include <iostream>
#include <math.h>

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
  m_weights = auto_parse::Vector(number);
  for(int i = 0; i < number; ++i)
    {
      double d;
      in >> d >> std::ws;
      m_weights[i] = d;
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear&
auto_parse::Forecast_linear::operator=(const Forecast_linear &rhs)
{
  m_weights = rhs.m_weights;
  return *this;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast_linear::Forecast_linear(const auto_parse::Vector& d) 
  : Forecast(),
    m_weights(d)
{
  int number_zeros = 0;
  for(unsigned int i = 0; i < d.size(); ++i)
    {
      if(isnan(m_weights[i]))
	{
	  m_weights[i] = 0;
	  ++number_zeros ;
	}
    };
  if(number_zeros != 0)
    std::cout << key() << " zeroed out " << number_zeros << " / " << m_weights.size() << " parameters which were nans." << std::endl;
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
void
auto_parse::Forecast_linear:: tweak(const Forecast& forecast_other, double movement) 
{  // movement = 1 means replace old with new, movement=0 means use old
  const Forecast_linear& other = dynamic_cast<const Forecast_linear&>(forecast_other);
#ifdef AVOID_EIGEN
  for(int i = 0; i < m_weights.size(); ++i)
    m_weights[i] = (1 - movement) * m_weights[i] + movement * other.m_weights[i];
#else
  m_weights = (1 - movement) * m_weights + movement * other.m_weights;
#endif
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

double
auto_parse::Forecast_linear::operator()(const auto_parse::Vector& row) const
{
  assert(row.size() == m_weights.size());
#ifdef AVOID_EIGEN
  assert(m_weights.size() == row.size());
  double result = 0;
  for(int i = 0; i < m_weights.size(); ++i)
    result += m_weights[i] * row[i];
#else
  double result = m_weights.transpose() * row;
#endif
  assert(!isnan(result));
  return result;
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

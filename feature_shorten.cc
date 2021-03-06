// -*- c++ -*-


#include "feature_shorten.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Shorten::~Shorten()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shorten::Shorten(const Feature& x, int length)
  :
  mp_long(x.clone()),
  m_dimension(length)
{
  assert(m_dimension <= x.dimension());  // yes, you can shorten a 10 long to a 10 long
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shorten::Shorten(const Shorten& other)
  :
  Feature(other),
  mp_long(other.mp_long->clone()),
  m_dimension(other.m_dimension)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shorten::Shorten(std::istream& in)
  :
  Feature(),
  mp_long(),
  m_dimension()
{
  in >> m_dimension >> std::ws;
  mp_long = Feature::restore(in);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shorten*
auto_parse::Shorten::clone() const
{
  return new Shorten(*mp_long, m_dimension);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Shorten*
auto_parse::Shorten::private_restore(std::istream& in) const
{
  return new Shorten(in);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

auto_parse::Vector
auto_parse::Shorten::operator()(const auto_parse::LR& parser) const
{
#ifdef AVOID_EIGEN
  Vector result(m_dimension);
  Vector tmp = (*mp_long)(parser);
  for(int i = 0; i < m_dimension; ++i)
    result[i] = tmp[i];
  return result;
#else
  return (*mp_long)(parser).head(m_dimension);
#endif
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::vector<std::string>
auto_parse::Shorten::variable_names() const
{
  std::vector<std::string> result;
  std::vector<std::string> names = (*mp_long).variable_names();
  for(int i = 0; i < m_dimension; ++i)
      {
	std::stringstream s;
	s << "head_" << m_dimension << "_" << names[i];
	result.push_back(s.str());
      };
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::string
auto_parse::Shorten::name() const
{
  std::stringstream s;
  s << "Shorten (to length " << m_dimension << ") version of " << mp_long->name() << "'.";
  return s.str();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int
auto_parse::Shorten::dimension() const
{
  return m_dimension;
}
////////////////////////////////////////////////////////////////////////////////////////////



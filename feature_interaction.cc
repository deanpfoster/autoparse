// -*- c++ -*-

#include "feature_interaction.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Interaction::~Interaction()
{
  delete mp_x1;
  delete mp_x2;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Interaction::Interaction(const Feature& x1, const Feature& x2)
  :
  Feature(),
  mp_x1(x1.clone()),
  mp_x2(x2.clone()),
  m_dimension(x1.dimension() * x2.dimension())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Interaction::Interaction(std::istream& in)
  :
  Feature(),
  mp_x1(Feature::restore(in)),
  mp_x2(Feature::restore(in)),
  m_dimension(mp_x1->dimension() * mp_x2->dimension())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Interaction::Interaction(const auto_parse::Interaction& other)
  :
  Feature(other),
  mp_x1(other.mp_x1->clone()),
  mp_x2(other.mp_x2->clone()),
  m_dimension(other.m_dimension)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Interaction*
auto_parse::Interaction::clone() const
{
  return new Interaction(*mp_x1, *mp_x2);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Interaction*
auto_parse::Interaction::private_restore(std::istream& in) const
{
  return new Interaction(in);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
Eigen::VectorXd
auto_parse::Interaction::operator()(const auto_parse::LR& parser) const
{
  Eigen::VectorXd v_x1 = (*mp_x1)(parser);
  Eigen::VectorXd v_x2 = (*mp_x2)(parser);
  Eigen::MatrixXd outer_product = v_x1 * (v_x2.transpose());
  Eigen::Map<Eigen::VectorXd> result(outer_product.data(),dimension());
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>
auto_parse::Interaction::variable_names() const
{
  std::vector<std::string> result;
  std::vector<std::string> n1s = mp_x1->variable_names();
  std::vector<std::string> n2s = mp_x2->variable_names();
  for(std::string n1 : n1s)
    for(std::string n2 : n2s)
      {
	std::stringstream s;
	if(n1 == n2)
	  s << n1 << "_squared";
	else
	  s << n1 << "_x_" << n2;
	result.push_back(s.str());
      }
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Interaction::name() const
{
  std::stringstream s;
  if(mp_x1->name() == mp_x2->name())
    s << "The square of `" << mp_x1->name() << "'.";
  else
    s << "`" << mp_x1->name()
      << "' times `" << mp_x2->name() << "'.";
  return s.str();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Interaction::dimension() const
{
  return m_dimension;
}
////////////////////////////////////////////////////////////////////////////////////////////



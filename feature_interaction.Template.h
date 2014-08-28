// -*- c++ -*-


#include "feature_interaction.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

template<class T1, class T2>
auto_parse::Interaction<T1, T2>::~Interaction()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T1, class T2>
auto_parse::Interaction<T1, T2>::Interaction(const T1& x1, const T2& x2)
  :
  m_x1(x1),
  m_x2(x2),
  m_dimension(x1.dimension() * x2.dimension())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T1, class T2>
auto_parse::Interaction<T1, T2>*
auto_parse::Interaction<T1, T2>::clone() const
{
  return new Interaction<T1, T2>(m_x1, m_x2);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
template<class T1, class T2>
Eigen::VectorXd
auto_parse::Interaction<T1, T2>::operator()(const auto_parse::LR& parser) const
{
  Eigen::VectorXd v_x1 = m_x1(parser);
  Eigen::VectorXd v_x2 = m_x2(parser);
  Eigen::MatrixXd outer_product = v_x1 * (v_x2.transpose());
  Eigen::Map<Eigen::VectorXd> result(outer_product.data(),dimension());
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T1, class T2>
std::vector<std::string>
auto_parse::Interaction<T1, T2>::variable_names() const
{
  std::vector<std::string> result;
  std::vector<std::string> n1s = m_x1.variable_names();
  std::vector<std::string> n2s = m_x2.variable_names();
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
template<class T1, class T2>
std::string
auto_parse::Interaction<T1, T2>::name() const
{
  std::stringstream s;
  if(m_x1.name() == m_x2.name())
    s << "The square of `" << m_x1.name() << "'.";
  else
    s << "`" << m_x1.name()
      << "' times `" << m_x2.name() << "'.";
  return s.str();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T1, class T2>
int
auto_parse::Interaction<T1, T2>::dimension() const
{
  return m_dimension;
}
////////////////////////////////////////////////////////////////////////////////////////////



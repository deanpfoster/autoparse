// -*- c++ -*-


#include "feature_eigenwords.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Feature_eigenwords::~Feature_eigenwords()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_eigenwords::Feature_eigenwords(const Eigenwords& e)
  :
  Feature(),
  m_eigenwords(e)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_eigenwords::Feature_eigenwords(const auto_parse::Feature_eigenwords& other)
  :
  Feature(other),
  m_eigenwords(other.m_eigenwords)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_eigenwords*
auto_parse::Feature_eigenwords::clone() const
{
  return new Feature_eigenwords(*this); 
};

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
Eigen::VectorXd
auto_parse::Feature_eigenwords::operator()(const auto_parse::LR& parser) const
{
  return m_eigenwords(*(parser.stack_top()));
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>::iterator
auto_parse::Feature_eigenwords::set_names(std::vector<std::string>::iterator i) const
{
  *i = "eigenwords";
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Feature_eigenwords::name() const
{
  return "Full sentence length.";
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Feature_eigenwords::dimension() const
{
  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////



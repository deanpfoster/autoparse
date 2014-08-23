// -*- c++ -*-


#include "feature_stack_size.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Feature_stack_size::~Feature_stack_size()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_stack_size::Feature_stack_size()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_stack_size*
auto_parse::Feature_stack_size::clone() const
{
  return new Feature_stack_size(); // can use default constructor since no state (except v-table)
};


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors


Eigen::VectorXd
auto_parse::Feature_stack_size::operator()(const auto_parse::LR& parser) const
{
  Eigen::VectorXd result(1);
  result[0] = parser.stack_size();
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>::iterator
auto_parse::Feature_stack_size::set_names(std::vector<std::string>::iterator i) const
{
  *i = "stack_size";
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Feature_stack_size::name() const
{
  return "Size of current stack.";
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Feature_stack_size::dimension() const
{
  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////



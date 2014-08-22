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
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
std::vector<double>::iterator
auto_parse::Feature_stack_size::set_values(std::vector<double>::iterator i,
					   const auto_parse::LR& parser) const
{
  *i = parser.stack_size();
  ++i;
  return i;
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



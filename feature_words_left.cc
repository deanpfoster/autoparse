// -*- c++ -*-


#include "feature_words_left.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Feature_words_left::~Feature_words_left()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_words_left::Feature_words_left()
{
};
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
std::vector<double>::iterator
auto_parse::Feature_words_left::set_values(std::vector<double>::iterator i,
					   const auto_parse::LR& parser) const
{
  *i = parser.number_words_left();
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>::iterator
auto_parse::Feature_words_left::set_names(std::vector<std::string>::iterator i) const
{
  *i = "num_words_left";
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Feature_words_left::name() const
{
  return "number of words left in the sentence";
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Feature_words_left::dimension() const
{
  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////



// -*- c++ -*-


#include "feature_sentence_length.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Feature_sentence_length::~Feature_sentence_length()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_sentence_length::Feature_sentence_length()
{
};
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
std::vector<double>::iterator
auto_parse::Feature_sentence_length::set_values(std::vector<double>::iterator i,
					   const auto_parse::LR& parser) const
{
  const Words& sentence = parser.parse().sentence();;
  *i = sentence.end() - sentence.begin();
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>::iterator
auto_parse::Feature_sentence_length::set_names(std::vector<std::string>::iterator i) const
{
  *i = "sentence_length";
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Feature_sentence_length::name() const
{
  return "Full sentence length.";
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Feature_sentence_length::dimension() const
{
  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////



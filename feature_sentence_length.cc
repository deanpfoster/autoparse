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
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_sentence_length*
auto_parse::Feature_sentence_length::clone() const
{
  return new Feature_sentence_length(); // can use default constructor since no state (except v-table)
};

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
Eigen::VectorXd
auto_parse::Feature_sentence_length::operator()(const auto_parse::LR& parser) const
{
  const Words& sentence = parser.parse().sentence();;
  Eigen::VectorXd result(1);
  result[0] = sentence.end() - sentence.begin();
  return result;
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



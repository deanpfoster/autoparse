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
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_words_left*
auto_parse::Feature_words_left::clone() const
{
  return new Feature_words_left(); // can use default constructor since no state (except v-table)
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
Eigen::VectorXd
auto_parse::Feature_words_left::operator()(const auto_parse::LR& parser) const
{
  Eigen::VectorXd result(1);
  result[0] = parser.number_words_left();
  return result;
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



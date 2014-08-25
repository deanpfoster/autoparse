// -*- c++ -*-


#include "feature_one_dimensional.h"
#include "lr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

template<class T>
auto_parse::Feature_one_dimensional<T>::~Feature_one_dimensional()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_one_dimensional<T>::Feature_one_dimensional()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_one_dimensional<T>*
auto_parse::Feature_one_dimensional<T>::clone() const
{
  return new Feature_one_dimensional<T>(); // can use default constructor since no state (except v-table)
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
template<class T>
Eigen::VectorXd
auto_parse::Feature_one_dimensional<T>::operator()(const auto_parse::LR& parser) const
{
  Eigen::VectorXd result(1);
  result[0] = T()(parser);
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
std::vector<std::string>::iterator
auto_parse::Feature_one_dimensional<T>::set_names(std::vector<std::string>::iterator i) const
{
  *i = T().variable_name();
  ++i;
  return i;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
std::string
auto_parse::Feature_one_dimensional<T>::name() const
{
return T().descriptive_name();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
int
auto_parse::Feature_one_dimensional<T>::dimension() const
{
  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////



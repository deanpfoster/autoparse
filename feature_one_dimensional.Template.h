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
  :Feature()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_one_dimensional<T>::Feature_one_dimensional(const auto_parse::Feature_one_dimensional<T>&)
  :Feature()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_one_dimensional<T>::Feature_one_dimensional(std::istream&)
  :Feature()
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
template<class T>
auto_parse::Feature_one_dimensional<T>*
auto_parse::Feature_one_dimensional<T>::private_restore(std::istream& in) const
{
  return new Feature_one_dimensional<T>(in);
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
template<class T>
auto_parse::Vector
auto_parse::Feature_one_dimensional<T>::operator()(const auto_parse::LR& parser) const
{
  Vector result(1);
  result[0] = T()(parser);
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
std::vector<std::string>
auto_parse::Feature_one_dimensional<T>::variable_names() const
{
  std::vector<std::string> result(dimension());
  result[0] = T().variable_name();
  return result;
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



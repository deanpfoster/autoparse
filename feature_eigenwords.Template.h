// -*- c++ -*-


#include "feature_eigenwords.h"
#include "lr.h"
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors
template<class T>
auto_parse::Feature_eigenwords<T>::~Feature_eigenwords()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_eigenwords<T>::Feature_eigenwords(const Eigenwords& e)
  :
  Feature(),
  m_eigenwords(e)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_eigenwords<T>::Feature_eigenwords(const auto_parse::Feature_eigenwords<T>& other)
  :
  Feature(other),
  m_eigenwords(other.m_eigenwords)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_eigenwords<T>::Feature_eigenwords(std::istream& in)
  :
  Feature(in),
  m_eigenwords(in)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_eigenwords<T>*
auto_parse::Feature_eigenwords<T>::clone() const
{
  return new Feature_eigenwords<T>(*this); 
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
auto_parse::Feature_eigenwords<T>*
auto_parse::Feature_eigenwords<T>::private_restore(std::istream& in) const
{
  return new Feature_eigenwords<T>(in); 
};

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
template<class T>
auto_parse::Vector
auto_parse::Feature_eigenwords<T>::operator()(const auto_parse::LR& parser) const
{
  return m_eigenwords(T()(parser));
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
std::vector<std::string>
auto_parse::Feature_eigenwords<T>::variable_names() const
{
  std::vector<std::string> result(dimension());
  auto i = result.begin();
  for(int j = 0; j < m_eigenwords.dimension(); ++j)
    {
      std::stringstream s;
      s << T().name() << "_eigenwords_" << j;
      *i = s.str();
      ++i;
    }
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
std::string
auto_parse::Feature_eigenwords<T>::name() const
{
  std::stringstream s;
  s << T().name() << " as an Eigenword.";
  return s.str();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
template<class T>
int
auto_parse::Feature_eigenwords<T>::dimension() const
{
  return m_eigenwords.dimension();
}
////////////////////////////////////////////////////////////////////////////////////////////



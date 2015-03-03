// -*- c++ -*-


#include "feature_generator.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <iterator>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Feature_generator::~Feature_generator()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator::Feature_generator()
  :
  m_number_features(0),
  m_features()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator::Feature_generator(const Feature_generator & other)
  :
  m_number_features(other.m_number_features),
  m_features()
{
    for(auto i = other.m_features.begin(); i != other.m_features.end(); ++i)
    {
      m_features.push_back((*i)->clone());
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator::Feature_generator(std::istream & in)
  :
  m_number_features(),
  m_features()
{
  in >> m_number_features >> std::ws;
  for(int i = 0; i < m_number_features; ++i)
    {
      m_features.push_back(Feature::restore(in));
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator&
auto_parse::Feature_generator::add(const std::initializer_list<Feature*>& cool_shit)
{
  std::vector<Feature*> tmp = cool_shit;
  for(auto i = cool_shit.begin(); i != cool_shit.end(); ++i)
    {
      m_number_features += (*i)->dimension();
      m_features.push_back((*i)->clone());
    }
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator&
auto_parse::Feature_generator::add(const Feature& f)
{
  m_number_features += f.dimension();
  m_features.push_back(f.clone());
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator&
auto_parse::Feature_generator::add(const std::vector<Feature*>& vec)
{
  for(auto i = vec.begin(); i != vec.end(); ++i)
    {
      m_number_features += (*i)->dimension();
      m_features.push_back((*i)->clone());
    }
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator&
auto_parse::Feature_generator::add(const auto_parse::Feature_generator& gen)
{
  return add(gen.m_features);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
void
auto_parse::Feature_generator::print_on(std::ostream & ostrm) const
{
  ostrm << "Features used:\n\n\t";
  std::vector<std::string> sum = feature_summaries();
  std::copy(sum.begin(), sum.end(), std::ostream_iterator<std::string>(ostrm,"\n\t"));
  std::vector<std::string> all_names = feature_names();
  ostrm << "\n";
  bool first = true;
  for(std::string a : all_names)
    {
      if(!first)
	ostrm << ", ";
      ostrm << a;
      first = false;
    }
  ostrm << std::endl<< std::endl;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Eigen::VectorXd
auto_parse::Feature_generator::operator()(const auto_parse::LR& lr) const
{
  return to_VectorXd(features(lr));
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Vector
auto_parse::Feature_generator::features(const LR& parser) const
{
  Vector result(m_number_features);

  int current_location = 0;
  for(auto i  = m_features.begin(); i != m_features.end(); ++i)
    {
      int number_to_add = (*i)->dimension();
      assert(current_location + number_to_add <= m_number_features);
#ifdef AVOID_EIGEN
      Vector tmp = (**i)(parser);
      assert(tmp.size() == number_to_add);
      for(int i = 0; i < tmp.size(); ++i)
	result[current_location + i] = tmp[i];
#else
      result.segment(current_location,number_to_add) = (**i)(parser);
#endif
      current_location += number_to_add;
    }
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>
auto_parse::Feature_generator::feature_names() const
{
  std::vector<std::string> result(m_number_features);
  auto current_location = result.begin();
  for(auto i = m_features.begin(); i != m_features.end(); ++i)
    {
      std::vector<std::string> tmp = (*i)->variable_names();
      current_location =std::copy(tmp.begin(), tmp.end(), current_location);
    };
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>
auto_parse::Feature_generator::feature_summaries() const
{
  std::vector<std::string> result;
  for(auto i = m_features.begin(); i != m_features.end(); ++i)
    result.push_back((*i)->name());
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Feature_generator & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::ostream & operator<<(std::ostream & ostrm, const std::vector<std::string> & variables)
{
  for(auto w: variables)
    ostrm << w << " : ";
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

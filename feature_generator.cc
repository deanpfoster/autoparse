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
auto_parse::Feature_generator::Feature_generator(const std::vector<Feature*>& vec)
  :
  m_number_features(0),
  m_features(vec)
{
  for(auto i = m_features.begin(); i != m_features.end(); ++i)
    m_number_features += (*i)->dimension();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator::Feature_generator(const std::initializer_list<Feature*>& cool_shit)
  :
  m_number_features(0),
  m_features(cool_shit)
{
  for(auto i = m_features.begin(); i != m_features.end(); ++i)
    m_number_features += (*i)->dimension();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature_generator::Feature_generator(const Feature_generator & other)
  :
  m_number_features(other.m_number_features),
  m_features(other.m_features)
{
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
auto_parse::Feature_generator::features(const LR& parser) const
{
  Eigen::VectorXd result(m_number_features);
  int current_location = 0;
  for(auto i  = m_features.begin(); i != m_features.end(); ++i)
    {
      int number_to_add = (*i)->dimension();
      result.segment(current_location,number_to_add) = (**i)(parser);
      current_location += number_to_add;
    }
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<std::string>
auto_parse::Feature_generator::feature_names() const
{
  std::vector<std::string> result(m_number_features,"");
  auto current_location = result.begin();
  for(auto i = m_features.begin(); i != m_features.end(); ++i)
    current_location = (*i)->set_names(current_location);
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

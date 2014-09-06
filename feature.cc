// -*- c++ -*-

#include "feature.h"
#include <Eigen/Core>
#include "assert.h"
#include <iostream>
#include <map>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using
////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

std::map<std::string, auto_parse::Feature*> all_features;

auto_parse::Feature::~Feature()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature::Feature()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature::Feature(const Feature & ) 
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Feature*
auto_parse::Feature::restore(std::istream & in) 
{
  std::string feature_name;
  getline(in,feature_name);
  in >> std::ws;
  if(all_features.find(feature_name) == all_features.end())
    std::cout << "Feature " << feature_name << " needs to have its registry called from main()." << std::endl;
  assert(all_features.find(feature_name) != all_features.end());
  return all_features.find(feature_name)->second->private_restore(in);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
auto_parse::Feature*
auto_parse::Feature::private_restore(std::istream&) const
{
  std::cout << "Oops: you need to write this yourself.  Please write a private_restore." << std::endl;
  assert(0);
  return this->clone();
}

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


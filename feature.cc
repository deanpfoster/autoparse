// -*- c++ -*-

#include "feature.h"
#include <Eigen/Core>
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using
////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

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

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
Eigen::VectorXd
auto_parse::Feature::operator()(const LR& parser) const
{
  std::vector<double> tmp(dimension(),0);
  set_values(tmp.begin(), parser);
  Eigen::VectorXd result(dimension());
  for(int i = 0; i < dimension(); ++i)
    result[i] = tmp[i];
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<double>::iterator
auto_parse::Feature:: set_values(std::vector<double>::iterator, const LR&) const
{
  std::cout << "You should call operator()" << std::endl;
  assert(0);
};


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


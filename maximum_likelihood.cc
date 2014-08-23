// -*- c++ -*-


#include "maximum_likelihood.h"

// put other includes here
#include "assert.h"
#include "dependency.h"
#include "likelihood.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using
////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Maximum_Likelihood::~Maximum_Likelihood()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_Likelihood::Maximum_Likelihood(const Transition_probability& left, const Transition_probability& right)
  :
  mp_left(left.clone()),
  mp_right(right.clone())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_Likelihood::Maximum_Likelihood(const Maximum_Likelihood & other)
  :
  mp_left(other.mp_left->clone()),
  mp_right(other.mp_right->clone())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

void
auto_parse::Maximum_Likelihood::operator()(const auto_parse::Dependency& parse)
{
  assert(parse.full_parse());
  for(auto i = parse.links().begin(); i != parse.links().end(); ++i)
    {
      if(i->first < i->second)
	mp_left->accumulate(*i->first, *i->second);
      else
	mp_right->accumulate(*i->first, *i->second);
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

auto_parse::Likelihood
auto_parse::Maximum_Likelihood::output() const
{
  return Likelihood(*mp_left, *mp_right);
}


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


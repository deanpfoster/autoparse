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

auto_parse::Maximum_likelihood::~Maximum_likelihood()
{
  delete mp_left;
  delete mp_right;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood::Maximum_likelihood(const Transition_probability& left, const Transition_probability& right)
  :
  mp_left(left.clone()),
  mp_right(right.clone())
{
  assert(mp_left);
  assert(mp_right);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood::Maximum_likelihood()
  :
  mp_left(0),
  mp_right(0)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood::Maximum_likelihood(const Maximum_likelihood & other)
  :
  mp_left(0),
  mp_right(0)
{
  assert(other.mp_right);  // why would we want to copy an empty object?
  if(other.mp_right)
    {
      mp_left = other.mp_left->clone();
      mp_right= other.mp_right->clone();
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood&
auto_parse::Maximum_likelihood::operator=(const Maximum_likelihood & other)
{
  delete(mp_left);
  delete(mp_right);
  assert(other.mp_left);  // why would we copy an empty MLE?
  mp_left = other.mp_left->clone();
  mp_right = other.mp_right->clone();
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

void
auto_parse::Maximum_likelihood::operator()(const auto_parse::Dependency& parse)
{
  assert(mp_right);
  assert(mp_left);
  if(!parse.full_parse())
    std::cout << "Warning:"  << parse << std::endl;
  for(auto i = parse.links().begin(); i != parse.links().end(); ++i)
    {
      if(i->first < i->second)
	mp_left->accumulate(*i->first, *i->second);
      else
	mp_right->accumulate(*i->first, *i->second);
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Maximum_likelihood::merge(const auto_parse::Maximum_likelihood& other)
{
  mp_left->merge(*other.mp_left);
  mp_right->merge(*other.mp_right);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

auto_parse::Likelihood
auto_parse::Maximum_likelihood::output() const
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


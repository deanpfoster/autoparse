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
  delete mp_root;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood::Maximum_likelihood(const Transition_probability& left,
						   const Transition_probability& right,
						   const Transition_probability& root)
  :
  mp_left(left.clone()),
  mp_right(right.clone()),
  mp_root(root.clone())
{
  assert(mp_left);
  assert(mp_right);
  assert(mp_root);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood::Maximum_likelihood()
  :
  mp_left(0),
  mp_right(0),
  mp_root(0)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood::Maximum_likelihood(const Maximum_likelihood & other)
  :
  mp_left(0),
  mp_right(0),
  mp_root(0)
{
  assert(other.mp_right);  // why would we want to copy an empty object?
  if(other.mp_right)
    {
      mp_left = other.mp_left->clone();
      mp_right= other.mp_right->clone();
      mp_root = other.mp_root->clone();
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Maximum_likelihood&
auto_parse::Maximum_likelihood::operator=(const Maximum_likelihood & other)
{
  delete(mp_left);
  delete(mp_right);
  delete(mp_root);
  assert(other.mp_left);  // why would we copy an empty MLE?
  mp_left = other.mp_left->clone();
  mp_right = other.mp_right->clone();
  mp_root = other.mp_root->clone();
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
      if(i->parent() < i->child())
	mp_left->accumulate(i->parent(), i->child(),parse.sentence());
      else
	mp_right->accumulate(i->parent(), i->child(),parse.sentence());
    }
  mp_root->accumulate(parse.sentence().end(), parse.root(), parse.sentence());
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Maximum_likelihood::merge(const auto_parse::Maximum_likelihood& other)
{
  mp_left->merge(*other.mp_left);
  mp_right->merge(*other.mp_right);
  mp_root->merge(*other.mp_root);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

auto_parse::Likelihood
auto_parse::Maximum_likelihood::output() const
{
  Transition_probability* pl = mp_left->renormalize();
  Transition_probability* pr = mp_right->renormalize();
  Transition_probability* p0 = mp_root->renormalize();
  Likelihood result(*pl,*pr,*p0);
  delete pl;
  delete pr;
  delete p0;
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


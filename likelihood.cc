// -*- c++ -*-


#include "likelihood.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using
////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Likelihood::~Likelihood()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood::Likelihood(const Transition_probability& left, const Transition_probability& right)
  :
  mp_left(left.clone()),
  mp_right(right.clone())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood::Likelihood(const Likelihood & other)
  :
  mp_left(other.mp_left->clone()),
  mp_right(other.mp_right->clone())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Likelihood&
auto_parse::Likelihood::operator=(const Likelihood & rhs)
{
  delete mp_left;
  delete mp_right;
  mp_left = rhs.mp_left->clone();
  mp_right =rhs.mp_right->clone();
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
void
auto_parse::Likelihood::print_on(std::ostream & ostrm) const
{
  ostrm << "Likelihood model:" << std::endl;
  ostrm << "\t" << (*mp_left);
  ostrm << "\t" << (*mp_right);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double
auto_parse::Likelihood::operator()(const Dependency& parse) const
{
  //  assert(parse.full_parse());  // is this step as slow as it seems to be?
  double result = 0.0; // we will work with log likelihoods
  for(auto i = parse.links().begin(); i != parse.links().end(); ++i)
    {
      double delta = link_probability(*i);
      bool debugging = false;
      if(debugging)
	{
	  std::cout << *i->first;
	  if(i->first < i->second)
	    std::cout << " <-- ";
	  else
	    std::cout << " --> ";
	  std::cout << *i->second << " = " << delta << std::endl;
	};
      result += delta;
    }
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Decorated_dependency
auto_parse::Likelihood::decorate(const Dependency& parse, const Eigenwords& dictionary) const
{
  Decorated_dependency result(parse,dictionary);
  for(auto i = result.links().begin(); i != result.links().end(); ++i)
    result.describe_link(*i,link_probability(*i));
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double
auto_parse::Likelihood::link_probability(const Link& link) const
{
  double result = 0;
  if(link.first < link.second)
    result = (*mp_left)(*link.first, *link.second);
  else
    result = (*mp_right)(*link.first, *link.second);
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

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Likelihood & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

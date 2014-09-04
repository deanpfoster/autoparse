// -*- c++ -*-


#include "likelihood.h"
#include "utilities/string_handling.h"

#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Likelihood::~Likelihood()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood::Likelihood(const Transition_probability& left,
				   const Transition_probability& right,
				   const Transition_probability& root)
  :
  mp_left(left.clone()),
  mp_right(right.clone()),
  mp_root(root.clone())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood::Likelihood(const Likelihood & other)
  :
  mp_left(other.mp_left->clone()),
  mp_right(other.mp_right->clone()),
  mp_root(other.mp_root->clone())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Likelihood&
auto_parse::Likelihood::operator=(const Likelihood & rhs)
{
  delete mp_left;
  delete mp_right;
  delete mp_root;
  mp_left = rhs.mp_left->clone();
  mp_right =rhs.mp_right->clone();
  mp_root =rhs.mp_root->clone();
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
  ostrm << "\t" << (*mp_root);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double
auto_parse::Likelihood::operator()(const Dependency& parse) const
{
  //  assert(parse.full_parse());  // is this step as slow as it seems to be?
  double result = 0.0; // we will work with log likelihoods
  for(auto i = parse.links().begin(); i != parse.links().end(); ++i)
    {
      double delta = link_probability(*i,parse.sentence());
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
    };
  double root = (*mp_root)(parse.sentence().end(), parse.root(), parse.sentence());
  return result + root;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Eigen::VectorXd
auto_parse::Likelihood::pieces(const Dependency& parse) const
{
  Eigen::VectorXd result = Eigen::VectorXd::Zero(6);
  const Words& w = parse.sentence();
  int n = w.end() - w.begin();
  for(auto i = parse.links().begin(); i != parse.links().end(); ++i)
    {
      if(i->first < i->second)
	{
	  result[0] += (*mp_left)(i->first, i->second,w);
	  result[1] += 1.0;
	}
      else
	{
	  result[2] += (*mp_right)(i->first, i->second,w);
	  result[3] += 1.0;
	}
    };
  result[4] = (*mp_root)(w.end(), parse.root(), w);
  result[5] = 1;
  result = result / n;
  return result ;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Likelihood::summarize_pieces(const Eigen::VectorXd& pieces) const
{
  // generates string:  log(like) = 3.61 = 60\% lefts @ 4.00 + 40\% rights @ 3.00 + 1\% roots @ 10
  
  std::stringstream s;
  double average_left  = pieces[0] / pieces[1];
  double average_right = pieces[2] / pieces[3];
  double average_root  = pieces[4] / pieces[5];
  double total = pieces[0] + pieces[2] + pieces[4];

  s << "log(like) = " << abs_round(total,3)
    << " = " << relative_round(100*pieces[1],1) << "\\% lefts @ "  << abs_round(average_left ,3)
    << " + " << relative_round(100*pieces[3],1) << "\\% rights @ " << abs_round(average_right,3)
    << " + " << relative_round(100*pieces[5],1) << "\\% roots @ "  << abs_round(average_root,3);
  return s.str();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Decorated_dependency
auto_parse::Likelihood::decorate(const Dependency& parse, const Eigenwords& dictionary) const
{
  Decorated_dependency result(parse,dictionary);
  for(auto i = result.links().begin(); i != result.links().end(); ++i)
    result.describe_link(*i,link_probability(*i,parse.sentence()));
  double root = (*mp_root)(parse.sentence().end(), parse.root(), parse.sentence());
  result.describe_root(root);
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double
auto_parse::Likelihood::link_probability(const Link& link, const Words& w) const
{
  double result = 0;
  if(link.first == w.end())
    result = (*mp_root)(w.end(), link.second, w);
  else
    if(link.first < link.second)
      result = (*mp_left)(link.first, link.second,w);
    else
      result = (*mp_right)(link.first, link.second,w);
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

// -*- c++ -*-


#include "tp_iid.h"
#include <math.h>
// put other includes here
#include "assert.h"
#include <iostream>

static const double prior_total = 1.0;

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using


////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::TP_iid::~TP_iid()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_iid::TP_iid(const std::vector<double>& total, double scaling)
  : Transition_probability(),
    m_scaling(scaling),
    m_total(total) 
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_iid::TP_iid(int size,double scaling)
  : Transition_probability(),
    m_scaling(scaling),
    m_total(size,prior_total)  // Bayeisian prior
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_iid::TP_iid(const auto_parse::TP_iid& other)
  : Transition_probability(other),
    m_scaling(other.m_scaling),
    m_total(other.m_total) 
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_iid*
auto_parse::TP_iid::clone() const
{
  return new auto_parse::TP_iid(*this);
};
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_iid::accumulate(const Node&, const Node& c, const Words&) 
{
  m_total[c->as_index()] += 1;  
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_iid::accumulate(const Words& ) 
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_iid::merge(const Transition_probability& tp) 
{
  const TP_iid& other = dynamic_cast<const TP_iid&>(tp);
  for(unsigned int i = 0; i < m_total.size(); ++i)
    m_total[i] += other.m_total[i] - prior_total; // remove other prior
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_iid*
auto_parse::TP_iid::renormalize() const
{
  double total = 0;
  for(unsigned int i = 0; i < m_total.size(); ++i)
    total += m_total[i];
  auto result_vector = m_total;
  for(unsigned int i = 0; i < m_total.size(); ++i)
    result_vector[i] = m_total[i] / total;
  return new TP_iid(result_vector, m_scaling);
};


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
//
//  Likelihood calculation assumes that Y ~ N(X Beta, I).
//  So 2 * log(like) = |Y - X Beta|^2
//  This assumes that we have already computed X'X^-1 X'Y and shoved it in X'Y.
//
double
auto_parse::TP_iid::operator()(const auto_parse::Node&,
			       const auto_parse::Node& child,
			       const auto_parse::Words& ) const
{
  double prob_gen_child = m_total[child->as_index()];
  double log_gen = m_scaling * log(prob_gen_child);
  assert(!isnan(log_gen));
  return log_gen;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double
auto_parse::TP_iid::operator()( const auto_parse::Words& ) const
{
  return 0;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::TP_iid::print_on(std::ostream & ostrm) const
{
  ostrm << "IID transition matrix." << std::endl;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::TP_iid & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

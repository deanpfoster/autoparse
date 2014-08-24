// -*- c++ -*-


#include "contrast.h"
#include "row.h"
#include "suggest_alternative_history.h"
#include "redo_parse.h" 

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Contrast::~Contrast()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Contrast::Contrast(const Statistical_parse& parse,
			       const Likelihood& like,
			       const Feature_generator& gen)
  :m_parser(parse),
   m_likelihood(like),
   m_feature_generator(gen)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
void
auto_parse::Contrast::operator()(std::ostream& out, const Words& sentence) const
{
  Statistical_history h = m_parser(sentence);
  History prefix = suggest_alternative_history(h);  // truncates and modifies the history
  History h_prime = m_parser.finish(sentence, prefix);
  double l       = m_likelihood(redo_parse(sentence, h      ).parse());
  double l_prime = m_likelihood(redo_parse(sentence, h_prime).parse());
  History common = prefix;
  common.pop_back();
  Action a_prime = h_prime[common.size()];
  Action a = h[common.size()];
  out << rows(m_feature_generator,sentence,common, a, l, a_prime, l_prime);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::vector<auto_parse::Row>
auto_parse::Contrast::operator()(const Words& sentence) const
{
  Statistical_history h = m_parser(sentence);
  History prefix = suggest_alternative_history(h);  // truncates and modifies the history
  if(!check_legal(sentence, prefix)) return std::vector<auto_parse::Row>(0);
  History h_prime = m_parser.finish(sentence, prefix);
  double l       = m_likelihood(redo_parse(sentence, h      ).parse());
  double l_prime = m_likelihood(redo_parse(sentence, h_prime).parse());
  History common = prefix;
  common.pop_back();
  Action a_prime = h_prime[common.size()];
  Action a = h[common.size()];
  if(a == a_prime) return std::vector<auto_parse::Row>(0);
  return(rows(m_feature_generator,sentence,common, a, l, a_prime, l_prime));
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


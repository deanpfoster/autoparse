// -*- c++ -*-


#include "contrast.h"
#include "row.h"
#include "redo_parse.h" 
#include "utilities/z.h"
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
  :Contrast_helper(parse, gen),
   m_likelihood(like)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
std::vector<auto_parse::Row>
auto_parse::Contrast::operator()(const Words& sentence) const
{
  History h = m_parser(sentence);
  History alt;
  for(int i = 0; i < 10; ++i)  // make 10 attempts to find a legal alternative
    if(alt.size() == 0)
      alt =  suggest_alternative_history(sentence, h);
  if(alt.size() == 0)
    return std::vector<auto_parse::Row>(0);
    // truncates and modifies the history
  assert(check_legal(sentence, alt));
  Action a_prime = alt[alt.size()-1];
  Action a       = h  [alt.size()-1];
  assert(a != a_prime);
  History h_prime = m_parser.finish(sentence, alt);
  double l        = m_likelihood(redo_parse(sentence, h      ).parse());
  double l_prime  = m_likelihood(redo_parse(sentence, h_prime).parse());
  History common = alt;
  common.pop_back();
  return(rows(m_feature_generator,sentence,common, a, l, a_prime, l_prime));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


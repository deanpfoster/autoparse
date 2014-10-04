// -*- c++ -*-


#include "golden_contrast.h"
#include "row.h"
#include "redo_parse.h" 
#include "utilities/z.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Golden_contrast::~Golden_contrast()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Golden_contrast::Golden_contrast(const Statistical_parse& parse,
					     const Feature_generator& gen)
  :Contrast_helper(parse,gen)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
std::vector<auto_parse::Row>
auto_parse::Golden_contrast::operator()(const Gold_standard& g, const Words& sentence) const
{
  History h = m_parser(sentence);
  History alt;
  for(int i = 0; i < 5; ++i)  // make 5 attempts to find a legal alternative
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
  double l        = g(redo_parse(sentence, h      ).parse())/sentence.size();
  double l_prime  = g(redo_parse(sentence, h_prime).parse())/sentence.size();
  History common = alt;
  common.pop_back();
  return(rows(m_feature_generator,sentence,common, a, l, a_prime, l_prime));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


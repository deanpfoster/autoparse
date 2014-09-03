// -*- c++ -*-


#include "contrast.h"
#include "row.h"
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
std::vector<auto_parse::Row>
auto_parse::Contrast::operator()(const Words& sentence) const
{
  History h = m_parser(sentence);
  History alt = suggest_alternative_history(sentence, h);
  if(alt.size() == 0)
    return std::vector<auto_parse::Row>(0);
    // truncates and modifies the history
  assert(check_legal(sentence, alt));
  double l       = m_likelihood(redo_parse(sentence, h  ).parse());
  double l_prime = m_likelihood(redo_parse(sentence, alt).parse());
  History c = common(h,alt);
  Action a_prime = alt[c.size()];
  Action a = h[c.size()];
  assert(a != a_prime);
  if(a == a_prime) return std::vector<auto_parse::Row>(0);
  return(rows(m_feature_generator,sentence,c, a, l, a_prime, l_prime));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
auto_parse::History
auto_parse::Contrast::suggest_alternative_history(const Words& w,
						  const History& h) const
{
  if((m_count - 1000*(int(m_count/1000)) == 0) && (m_misses/m_count > .5))
    {
      std::cout << "suggest_alternative:" << m_count << " uses noise level exp(" << m_noise << ") yielding " << m_misses << " misses." << std::endl;
    }
  m_count ++;
  History result = m_parser(w,exp(m_noise));
  if(result == h)
    {
      m_noise += 1.0 / m_count;
      result =  m_parser(w,exp(m_noise)); // try again
    }
  else
    m_noise -= 1.0 / m_count;
  if(result == h)
    {
      m_misses ++;
      return History();
    }
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::History
auto_parse::Contrast::common(const History& h1,const History& h2) const
{
  assert(h1.size() == h2.size());
  assert(!(h1 == h2));
  History result;
  int i = 0;
  while(h1[i] == h2[i])
    result.push_back(h1[i++]);
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


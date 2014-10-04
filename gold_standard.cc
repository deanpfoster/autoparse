// -*- c++ -*-


#include "gold_standard.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Gold_standard::~Gold_standard()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Gold_standard::Gold_standard(const auto_parse::Dependency& d)
  :m_gold(d)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
double
auto_parse::Gold_standard::operator()(const auto_parse::Dependency & other) const
{
  assert(m_gold.sentence() == other.sentence()); // make sure we are usig the right sentences
  const std::vector<int>& our_parents = m_gold.parents();
  const std::vector<int>& their_parents = other.parents();
  double result = 0.0;
  for(unsigned int i = 0; i != our_parents.size(); ++i)
    {
      int our_link = our_parents[i];
      int their_link = their_parents[i];
      double distance = abs(our_link - their_link);
      result += 3.0/(1 + 2 * distance);
    };
  return result - 3 * m_gold.sentence().size(); // zero is great, minus is worse
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

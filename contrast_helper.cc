// -*- c++ -*-


#include "contrast_helper.h"
#include "row.h"
#include "redo_parse.h" 
#include "utilities/z.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Contrast_helper::~Contrast_helper()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Contrast_helper::Contrast_helper(const Statistical_parse& parse,
					     const Feature_generator& gen)
  :m_parser(parse),
   m_feature_generator(gen)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
auto_parse::History
auto_parse::Contrast_helper::suggest_alternative_history(const Words& w,
						  const History& h) const
{
  int n = h.size();
  int location = std::floor((n - .1) * my_random::U_thread_safe());
  assert(location >= 0);
  assert(location <= n - 1); 
  Action action_taken = h[location];
  History result;
  for(int i = 0; i < location; ++i)
    result.push_back(h[i]);
  LR parser = redo_parse(w,result);
  int number_alternatives = 0;
  for(Action a: m_parser.all_actions())
    number_alternatives += parser.legal(a);
  assert(number_alternatives != 0);
  if(number_alternatives == 1)
    return History();  // oops.  We missed.  No legal alternative available. 
  int which_one = (number_alternatives - 1) * my_random::U_thread_safe();
  assert(which_one <= number_alternatives);
  Action alternative_action = action_taken;
  for(Action a: m_parser.all_actions())
    if(parser.legal(a))
      if(a != action_taken)
	if(which_one-- == 0)
	  alternative_action = a;
  if(alternative_action == action_taken)
    {
      std::cout << "Reached 1 in a million times.  Why? " << action_taken << " @ " << number_alternatives << " , " << which_one << std::endl;
      assert(0);
      return History();  // return empty
    }
  result.push_back(alternative_action);
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::History
auto_parse::Contrast_helper::common(const History& h1,const History& h2) const
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


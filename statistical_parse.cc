// -*- c++ -*-


#include "statistical_parse.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Statistical_parse::~Statistical_parse()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Statistical_parse::Statistical_parse(const Model&)
  :
  m_model(model)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Statistical_parse::Statistical_parse(const Statistical_parse & other)
  :
  m_model(other.m_model)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors


auto_parse::Statistical_history
auto_parse::Statistical_parse::operator()(const Words& w) const
{
  LR parser(w);
  return do_actual_parse(&parser);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::History
auto_parse::Statistical_parse::continue(const Words& w, const History& h) const
{
  LR parser = redo_parse(w, h);
  Statistical_history sr = do_actual_parse();
  History rest = r; // slice off the statistical part
  History result = h;
  std::copy(rest.begin(), rest.end(), back_inserter(result));
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
auto_parse::Statistical_history
auto_parse::Statistical_parse::do_actual_parse(LR* p_parser) const
{
  Statistical_history result;
  bool done = false;
  while(!done)
    {
      Delta_forecasts suggestions = m_model(*p_parser);
      double max_delta = suggestions[head_reduce]; // this is always a legal action
      Action max_action = head_reduce;
      for (Action a :All_actions)
	{
	  if(suggestions[a] > max_delta)
	    {
	      if(parser->legal(a))
		{
		  max_delta = suggestions[a];
		  max_action = a;
		}
	      else
		suggestions[a] = -1e10;
	    }
	}
      result.add(max_action, suggestions);
      parser->take_action(a);
      if(a == head_reduction)
	{
	  done = true;
	  assert(parser->parse().full_parse());
	};
    }
  return(result);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


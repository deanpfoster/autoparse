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
      Value_of_forecasts values = m_model(*p_parser);
      for(Action a : all_actions)
	if(!parser->legal(a))
	  values[a] = -1e10;
      Action candidate = values.best_action();
      assert(parser->legal(candidate));
      result.add(candidate, values);
      parser->take_action(candidate);
      if(candidate == head_reduction)
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


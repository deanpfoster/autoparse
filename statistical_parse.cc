// -*- c++ -*-


#include "statistical_parse.h"

// put other includes here
#include "assert.h"
#include <iterator>
#include <iostream>
#include "lr.h"
#include "redo_parse.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Statistical_parse::~Statistical_parse()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Statistical_parse::Statistical_parse(const Model&model, const Feature_generator& gen,double noise)
  :
  m_model(model),
  m_generator(gen),
  m_noise(noise)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Statistical_parse::Statistical_parse(const Statistical_parse & other)
  :
  m_model(other.m_model),
  m_generator(other.m_generator),
  m_noise(other.m_noise)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors


auto_parse::Statistical_history
auto_parse::Statistical_parse::operator()(const Words& w, double noise_over_ride) const
{
  double noise = m_noise;
  if(noise_over_ride != -1)
    noise = noise_over_ride;
  LR parser(w);
  return do_actual_parse(&parser, noise);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::History
auto_parse::Statistical_parse::finish(const Words& w, const History& h, double noise_over_ride) const
{
  double noise = m_noise;
  if(noise_over_ride != -1)
    noise = noise_over_ride;
  LR parser = redo_parse(w, h);
  Statistical_history sr = do_actual_parse(&parser,noise);
  History rest = sr; // slice off the statistical part
  History result = h;
  for(auto i = rest.begin(); i != rest.end();++i)
    result.push_back(*i);
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
auto_parse::Statistical_history
auto_parse::Statistical_parse::do_actual_parse(LR* p_parser, double noise) const
{
  Statistical_history result;
  bool done = p_parser->parse().full_parse();
  while(!done)
    {
      Value_of_forecasts values = m_model(m_generator(*p_parser));
      for(Action a : all_actions)
	if(!p_parser->legal(a))
	  values[a] = -1e100;
      values.zero_second_best();
      Action candidate = values.smoothed_best_action(noise);
      assert(p_parser->legal(candidate));
      result.push_back(candidate, values);
      p_parser->take_action(candidate);
      if(candidate == Action::head_reduce)
	{
	  done = true;
	  assert(p_parser->parse().full_parse());
	};
    }
  return(result);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


// -*- c++ -*-


#include "statistical_parse.h"

// put other includes here
#include "assert.h"
#include <iterator>
#include <iostream>
#include "lr.h"
#include "redo_parse.h"
#include "value_of_forecasts.h"

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
auto_parse::Statistical_parse::Statistical_parse(std::istream& in)
  :
  m_model(in),
  m_generator(in),
  m_noise()
{
  in >> m_noise;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors


auto_parse::History
auto_parse::Statistical_parse::best_parse(const Words& w) const
{
  LR parser(w);
  return do_actual_parse(&parser, 0);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::History
auto_parse::Statistical_parse::operator()(const Words& w) const
{
  LR parser(w);
  return do_actual_parse(&parser, m_noise);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::History
auto_parse::Statistical_parse::best_parse_finish(const Words& w, const History& h) const
{
  return private_finish(w,h,0);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::History
auto_parse::Statistical_parse::finish(const Words& w, const History& h) const
{
  return private_finish(w,h,m_noise);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::History
auto_parse::Statistical_parse::private_finish(const Words& w, const History& h, double noise) const
{
  LR parser = redo_parse(w, h);
  History sr = do_actual_parse(&parser,noise);
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
auto_parse::History
auto_parse::Statistical_parse::do_actual_parse(LR* p_parser, double noise) const
{
  History result;
  bool done = p_parser->parse().full_parse();
  while(!done)
    {
      Value_of_forecasts values = m_model(m_generator.features(*p_parser));
      for(Action a : all_actions())
	if(!p_parser->legal(a))
	  values[a] = -1e100;
      values.zero_second_best();
      Action candidate = values.smoothed_best_action(noise);
      assert(p_parser->legal(candidate));
      result.push_back(candidate);
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


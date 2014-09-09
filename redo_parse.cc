// -*- c++ -*-

#include "redo_parse.h"
#include "history.h"
#include "lr.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

auto_parse::LR
auto_parse::redo_parse(const auto_parse::Words& w, const History& h)
{
  LR result(w);
  for(auto a : h)
    result.take_action(a);
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool
auto_parse::check_legal(const auto_parse::Words& w, const History& h)
{
  LR parser(w);
  for(auto a : h)
    {
      if(!parser.legal(a))
	return false;
      parser.take_action(a);
    };
  return true;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// -*- c++ -*-

#include "redo_parse.h"
#include "history.h"
#include "lr.h"
#include "assert.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

auto_parse::Dependency
auto_parse::redo_parse(const auto_parse::Words& w, const History& h)
{
  LR result(w);
  for(auto i = h.begin(); i != h.end(); ++i)
    switch(*i)
      {
      case auto_parse::Action::shift       : result.shift();       break;
      case auto_parse::Action::left_reduce : result.left_reduce(); break;
      case auto_parse::Action::right_reduce: result.right_reduce();break;
      case auto_parse::Action::head_reduce : result.head_reduce() ;break;
      default : assert(0);
      };
  return result.parse();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

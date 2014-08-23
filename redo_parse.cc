// -*- c++ -*-

#include "redo_parse.h"
#include "history.h"
#include "lr.h"
#include "assert.h"
#include "feature_generator.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

auto_parse::LR
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
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::write_row(std::ostream& out,
		      const auto_parse::Feature_generator& features,
		      const auto_parse::Words& w,
		      const auto_parse::History& common,
		      Action a1, double v1,
		      Action a2, double v2) 
{
  LR parse_state = redo_parse(w, common);
  Eigen::VectorXd X = features(parse_state);
  double diff = (v1 - v2)/2;
  // h1 row
  out << diff << ",\"" << a1 << "\"";
  for(int i = 0; i < X.size(); ++i)
    out << "," << X[i];
  out << std::endl;
  // h2 row
  out << -diff << ",\"" << a2 << "\"";
  for(int i = 0; i < X.size(); ++i)
    out << "," << X[i];
  out << std::endl;
};

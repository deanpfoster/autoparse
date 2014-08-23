// -*- c++ -*-

#ifndef INCLUDED_REDO_PARSE
#define INCLUDED_REDO_PARSE

#include "lr.h"
#include "history.h"

namespace auto_parse
{
  class Feature_generator;
  LR redo_parse(const auto_parse::Words&, const auto_parse::History&);

  void  write_row(std::ostream& out,
		  const auto_parse::Feature_generator& features,
		  const auto_parse::Words& w,
		  const auto_parse::History& common,
		  Action a1, double v1,
		  Action a2, double v2);
}

#endif

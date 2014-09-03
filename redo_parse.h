// -*- c++ -*-

#ifndef INCLUDED_REDO_PARSE
#define INCLUDED_REDO_PARSE

#include "lr.h"
#include "history.h"

namespace auto_parse
{
  LR redo_parse(const auto_parse::Words&, const auto_parse::History&);
  bool check_legal(const auto_parse::Words&, const auto_parse::History&);
}

#endif

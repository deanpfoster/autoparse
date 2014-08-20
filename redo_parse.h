// -*- c++ -*-

#ifndef INCLUDED_REDO_PARSE
#define INCLUDED_REDO_PARSE

#include "dependency.h"
#include "history.h"

namespace auto_parse
{
  Dependency redo_parse(const auto_parse::Words&, const auto_parse::History&);
}

#endif

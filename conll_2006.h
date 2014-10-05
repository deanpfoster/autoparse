// -*- c++ -*-

#ifndef INCLUDED_CONLL_2006
#define INCLUDED_CONLL_2006

#include "word.h"
#include "dependency.h"
#include <string>
#include <fstream>

namespace auto_parse
{
  Dependency read_conll(std::istream&, const Lexicon& );
  void write_conll(const Dependency&, std::ostream&) ;
}

#endif

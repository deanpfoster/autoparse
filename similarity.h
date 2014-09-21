// -*- c++ -*-

#ifndef INCLUDED_SIMILARITY
#define INCLUDED_SIMILARITY

#include <iosfwd>

namespace auto_parse
{
  class Dependency;
  double fraction_crosses(const Dependency&,const Dependency&);
  double similarity(const Dependency&,const Dependency&);
}


#endif

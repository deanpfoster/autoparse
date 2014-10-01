// -*- c++ -*-

#ifndef INCLUDED_SIMILARITY
#define INCLUDED_SIMILARITY

#include <iosfwd>

//
//   ???  NOTE: This ends up being an n^2 algorithm when it should be only n log(n).
//   If n ever starts getting larger than about 30, we should care.


namespace auto_parse
{
  class Dependency;
  double fraction_crosses(const Dependency&,const Dependency&);
  double similarity(const Dependency&,const Dependency&);
}


#endif

// -*- c++ -*-

#ifndef INCLUDED_INTERVAL
#define INCLUDED_INTERVAL

#include <iosfwd>
#include <vector>
#include <set>
#include "dependency.h"

namespace auto_parse
{
  class Interval
  {
  public:
    Interval(int,int);
    // ACCESSORS
    bool cross(const Interval&) const;
    std::string cross_check(const Interval&) const;
  private:
    int m_left, m_right;
  };

  class Intervals
  {
  public:
    // CONSTRUCTORS
    Intervals(const Dependency &);
    // ACCESSORS
    int count_crosses(const Intervals&) const;
    private:
      std::vector<Interval> m_intervals;
  };
};

#endif

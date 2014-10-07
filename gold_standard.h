// -*- c++ -*-

#ifndef INCLUDED_GOLD_STANDARD
#define INCLUDED_GOLD_STANDARD

#include "dependency.h"

namespace auto_parse
{
  class Gold_standard
  {
  public:
    // CONSTRUCTORS
    ~Gold_standard();
    Gold_standard(const auto_parse::Dependency&);
    Gold_standard(const Gold_standard &other):m_gold(other.m_gold){};

    // MANIPULATORS
    // ACCESSORS
    double operator()(const auto_parse::Dependency&) const;
    Dependency standard() const{return m_gold;}; // used only for debugging I exect

  private:
    Gold_standard& operator=(const Gold_standard &); // Don't delete this.
    Dependency m_gold;
  };
}

#endif

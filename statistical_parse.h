// -*- c++ -*-

#ifndef INCLUDED_STATISTICAL_PARSE
#define INCLUDED_STATISTICAL_PARSE

#include <iosfwd>
#include "model.h"
#include "statistical_history.h"
#include "word.h"

namespace auto_parse
{
  class Statistical_parse
  {
  public:
    // CONSTRUCTORS
    ~Statistical_parse();
    Statistical_parse(const Model&);

    // MANIPULATORS
    // ACCESSORS

    Statistical_history operator()(const Words&) const; 
    History finish(const Words&, const History& prefix) const; 

  protected:
  private:
    Statistical_history do_actual_parse(LR*) const;
    Model m_model;
    Statistical_parse(const Statistical_parse &);            // Don't delete this.
    Statistical_parse& operator=(const Statistical_parse &); // Don't delete this.
  };
}

#endif

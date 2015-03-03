// -*- c++ -*-

#ifndef INCLUDED_LEXICON_GENERATOR
#define INCLUDED_LEXICON_GENERATOR

#include "word.h"

///////////////////////////////////////////////////////////////////////////
//
//  This class needs explaining.  It is evil code in that it pretends
//  that every word is in the lexicon and simply adds new words as they
//  arrive.  The problem is that it does it by modifying the underlieing
//  data: m_index and m_words.  So it breaks const since the operator()
//  claims to be const but in fact it adds words.
//
///////////////////////////////////////////////////////////////////////////

namespace auto_parse
{
  class Lexicon_generator: public Lexicon
  {
  public:
    // CONSTRUCTORS
    virtual ~Lexicon_generator();
    Lexicon_generator();

    // MANIPULATORS
    // ACCESSORS
    virtual int operator()(const std::string& word) const;

  private:
    Lexicon_generator(const Lexicon_generator &);            // Don't delete this.
    Lexicon_generator& operator=(const Lexicon_generator &); // Don't delete this.
  };
}

#endif

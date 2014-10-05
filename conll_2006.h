// -*- c++ -*-

#ifndef INCLUDED_CONLL_2006
#define INCLUDED_CONLL_2006

#include "word.h"
#include "dependency.h"
#include <string>
#include <fstream>

namespace auto_parse
{
  class Conll_2006
  {
  public:
    // CONSTRUCTORS
    ~Conll_2006();
    Conll_2006(const std::string&, const Lexicon* p_l);

    // MANIPULATORS
    Dependency next_sentence();
    void reset();

    // ACCESSORS
    bool eof() const;
    void write_parse(const Dependency&, std::ostream&) const;
  protected:

  private:
    std::string m_file_name;
    std::ifstream m_input;
    const Lexicon* mp_lexicon;
  };
}

#endif

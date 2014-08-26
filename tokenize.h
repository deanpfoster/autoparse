// -*- c++ -*-

#ifndef INCLUDED_TOKENIZE
#define INCLUDED_TOKENIZE

#include "word.h"
#include <string>
#include <fstream>

namespace auto_parse
{
  class Tokenize
  {
  public:
    // CONSTRUCTORS
    ~Tokenize();
    Tokenize(const std::string&);

    // MANIPULATORS
    auto_parse::Words next_sentence();

    void reset();

    // ACCESSORS
    bool eof() const;
  protected:

  private:
    std::string m_file_name;
    std::ifstream m_input;
  };
}

#endif

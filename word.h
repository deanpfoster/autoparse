// -*- c++ -*-

#ifndef INCLUDED_WORD
#define INCLUDED_WORD

#include <iosfwd>
#include <string>
#include <vector>

namespace auto_parse
{
  typedef std::string Word;
  typedef std::vector<Word>     Words;
  typedef Words::const_iterator Node; 
  typedef Words::const_iterator const_word_iterator;
}

//  Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
auto_parse::Words
operator+(const auto_parse::Words& W, auto_parse::Word w); 

std::ostream& operator<<(std::ostream &,const auto_parse::Words &);




#endif

// -*- c++ -*-

#ifndef INCLUDED_WORD
#define INCLUDED_WORD

#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace auto_parse
{
  class Lexicon
  {
  public:
    Lexicon(const std::set<std::string>& map);
    Lexicon(const std::map<std::string,int>& map);
    // ACCESSORS
    int operator()(const std::string& word) const;
    std::string operator()(int) const;
  private:
    std::map<std::string, int> m_index;
    std::vector<std::string> m_words;
  };

  class Word
  {
  public:
    Word();
    Word(const std::string&);
    Word(const Word&);
    Word& operator=(const Word&);
    bool operator==(const Word&rhs) const{return m_word == rhs.m_word;};

    // ACCESSORS
    void print_on(std::ostream&) const;
    std::string convert_to_string() const;

  private:
    std::string m_word;
  };

  typedef std::vector<Word>     Words;
  typedef Words::const_iterator Node; 
  typedef Words::const_iterator const_word_iterator;

  Words reverse(const Words&);
  std::vector<Words> reverse(const std::vector<Words>&);


}

//  Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
auto_parse::Words
operator+(const auto_parse::Words& W, std::string w); 

std::ostream& operator<<(std::ostream &,const auto_parse::Words &);


#endif

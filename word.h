// -*- c++ -*-

#ifndef INCLUDED_WORD
#define INCLUDED_WORD

#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <assert.h>

namespace auto_parse
{
  class Lexicon
  {
  public:
    Lexicon(){};  // generates broken lexicon, please use operator= soon!
    Lexicon(const std::set<std::string>& map);
    Lexicon(const std::map<std::string,int>& map);
    Lexicon(const std::initializer_list<std::string>&);
    Lexicon(const std::vector<std::string>&);
    void operator=(const Lexicon& rhs){assert(m_index.size() == 0);m_index=rhs.m_index;m_words=rhs.m_words;};
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
    Word(const Lexicon&, const std::string&);
    Word(const Word&);
    Word& operator=(const Word&);
    bool operator==(const Word&rhs) const{return m_index == rhs.m_index;};

    // ACCESSORS
    void print_on(const Lexicon&, std::ostream&) const;
    std::string convert_to_string(const Lexicon&) const;

  private:
    int m_index;
  };

  class Words
  {
  public:
    typedef Word value_type;
    typedef std::vector<Word>::const_iterator const_iterator;
    typedef std::vector<Word>::const_reverse_iterator const_reverse_iterator;
    Words(const Lexicon*);
    Words(const Lexicon&,const std::string&); // inserts one word, useful in *.test.cc
    void push_back(const Word& w){m_words.push_back(w);};
    //ACCESSORS
    const Lexicon* p_lexicon() const{return mp_l;};
    const Lexicon& lexicon() const{return *mp_l;};
    const_iterator begin() const{return m_words.begin();};
    const_iterator end() const{return m_words.end();};
    const_reverse_iterator rbegin() const{return m_words.rbegin();};
    const_reverse_iterator rend() const{return m_words.rend();};
    int size() const{return m_words.size();};
  private:
    const Lexicon*           mp_l;
    std::vector<Word>  m_words;

  };

  typedef Words::const_iterator Node; 

  Words reverse(const Words&);
  std::vector<Words> reverse(const std::vector<Words>&);


}

//  Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
auto_parse::Words
operator+(const auto_parse::Words& W, std::string w); 

std::ostream& operator<<(std::ostream &,const auto_parse::Words &);


#endif

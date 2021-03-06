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
    virtual ~Lexicon(){}; 
    Lexicon(){};  // generates broken lexicon, please use operator= soon!
    Lexicon(const std::set<std::string>& map);
    Lexicon(const std::map<std::string,int>& map);
    Lexicon(const std::initializer_list<std::string>&);
    Lexicon(const std::vector<std::string>&);
    Lexicon(std::istream&);
    void operator=(const Lexicon& rhs){assert(m_index.size() == 0);m_index=rhs.m_index;m_words=rhs.m_words;};
    void set_cache_id(int id) const{m_cache_id=id;};// fake const
    // ACCESSORS
    std::string oov() const {return "<OOV>";};
    int oov_index() const {return m_index.find("<OOV>")->second;};
    virtual int operator()(const std::string& word) const;
    std::string operator()(int) const;
    int size() const{return m_words.size();};
    int cache_id() const{return m_cache_id;};
  protected:
    std::map<std::string, int> m_index;
    std::vector<std::string> m_words;
    mutable int m_cache_id;
  };

  class Word
  {
  public:
    Word();
    Word(const Lexicon&, const std::string&);
    Word(const Word&);
    Word& operator=(const Word&);
    bool operator==(const Word&rhs) const{return m_index == rhs.m_index;};
    bool operator<(const Word&rhs) const{return m_index < rhs.m_index;};

    // ACCESSORS
    void print_on(const Lexicon&, std::ostream&) const;
    std::string convert_to_string(const Lexicon&) const;
    int as_index() const {return m_index;};
  private:
    int m_index;
#ifndef NDEBUG
    std::string m_string;
#endif
  };

  
  // class Node
  // {
  // public:
  //   Node(const Node& other):m_value(other.m_value){};
  //   Node(const std::vector<Word>::const_iterator& other):m_value(other){};
  //   bool operator<(const Node& other) const{return m_value < other.m_value;};
  //   std::vector<Word>::const_iterator operator->() const{return m_value;};
  //   const Word& operator*() const{return *m_value;};
  //   Node operator+(int i) const{return m_value+i;};
  //   int operator-(const Node& n) const{return m_value-n.m_value;};
  //   void operator++(){++m_value;};
  //   bool operator!=(const Node& n) const{return m_value != n.m_value;};
  //   bool operator==(const Node& n) const{return m_value == n.m_value;};
  //   bool operator>=(const Node& n) const{return m_value >= n.m_value;};
  //   bool operator>(const Node& n) const{return m_value > n.m_value;};

  // private:
  //   std::vector<Word>::const_iterator m_value; 
  // };

  typedef std::vector<Word>::const_iterator Node;

  class Words
  {
  public:
    typedef Word value_type;
    typedef std::vector<Word>::const_iterator const_iterator;
    typedef std::vector<Word>::const_reverse_iterator const_reverse_iterator;
    Words(const Lexicon*);
    Words(const Lexicon&,const std::string&); // inserts one word, useful in *.test.cc
    void push_back(const Word& w){m_words.push_back(w);};
    void push_back(const std::string& w){m_words.push_back(Word(*mp_l,w));};
    //ACCESSORS
    Word operator[](int i) const{return m_words[i];};
    const Lexicon* p_lexicon() const{return mp_l;};
    const Lexicon& lexicon() const{return *mp_l;};
    Node begin() const{return Node(m_words.begin());};
    Node end() const{return m_words.end();};
    const_reverse_iterator rbegin() const{return m_words.rbegin();};
    const_reverse_iterator rend() const{return m_words.rend();};
    int size() const{return m_words.size();};
    bool operator==(const Words& rhs) const;
  private:
    const Lexicon*           mp_l;
    std::vector<Word>  m_words;

  };


  
  Words reverse(const Words&);
  std::vector<Words> reverse(const std::vector<Words>&);


}

//  Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
auto_parse::Words
operator+(const auto_parse::Words& W, std::string w); 

std::ostream& operator<<(std::ostream &,const auto_parse::Words &);


#endif

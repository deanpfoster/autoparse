// -*- c++ -*-

#include "word.h"
#include <iomanip>
#include <iterator>
#include <assert.h>

auto_parse::Lexicon::Lexicon(const std::set<std::string>& indexes)
  :
  m_index(),
  m_words(indexes.size()),
  m_cache_id(-1)
{
  for(auto i = begin(indexes), e = end(indexes); i != e; ++i)
    m_index.insert(m_index.end(),std::make_pair(*i,m_index.size()));
  assert(m_index.find(oov()) != m_index.end());
  for(auto i = begin(m_index), e = end(m_index); i != e; ++i)
    {
      assert(i->second >= 0);
      assert(i->second < int(m_index.size()));
      m_words[i->second] = i->first;
    }
}

auto_parse::Lexicon::Lexicon(const std::vector<std::string>& words)
  :
  m_index(),
  m_words(words),
  m_cache_id(-1)

{
  for(unsigned int i = 0;i < m_words.size(); ++i)
    m_index[m_words[i]] = i;
  assert(m_index.find(oov()) != m_index.end());
}

auto_parse::Lexicon::Lexicon(std::istream& in)
  :
  m_index(),
  m_words(),
  m_cache_id(-1)
{
  std::string start_string;
  getline(in, start_string);
  assert(start_string == "start<Lexicon>");
  int n;
  in >> n >> std::ws;
  m_words = std::vector<std::string>(n,"");
  for(int i = 0; i < n; ++i)
    in >> m_words[i] >> std::ws;
  for(unsigned int i = 0;i < m_words.size(); ++i)
    m_index[m_words[i]] = i;
  assert(m_index.find(oov()) != m_index.end());
  std::string end_string;
  getline(in, end_string);
  assert(end_string == "end<Lexicon>");

}

auto_parse::Lexicon::Lexicon(const std::initializer_list<std::string>& list)
  :
  m_index(),
  m_words(list.size()),
  m_cache_id(-1)
{
  for(auto i = begin(list), e = end(list); i != e; ++i)
    m_index.insert(m_index.end(),std::make_pair(*i,m_index.size()));
  assert(m_index.find(oov()) != m_index.end());
  for(auto i = begin(m_index), e = end(m_index); i != e; ++i)
    {
      assert(i->second >= 0);
      assert(i->second < int(m_index.size()));
      m_words[i->second] = i->first;
    }
}

auto_parse::Lexicon::Lexicon(const std::map<std::string, int>& indexes)
  :
  m_index(indexes),
  m_words(m_index.size()),
  m_cache_id(-1)
{
  assert(m_index.find(oov()) != m_index.end());
  for(auto i = begin(m_index), e = end(m_index); i != e; ++i)
    {
      assert(i->second >= 0);
      assert(i->second < int(m_index.size()));
      m_words[i->second] = i->first;
    }
}

int
auto_parse::Lexicon::operator()(const std::string& word) const
{
  auto location = m_index.find(word);
  if(location == m_index.end())
    location = m_index.find(oov());
  assert(location != m_index.end());
  return(location->second);
}
std::string
auto_parse::Lexicon::operator()(int location) const
{
  assert(location >= 0);
  assert(location < int(m_words.size()));
  return m_words[location];
}

auto_parse::Word::Word()
  :
  m_index(-1)
#ifndef NDEBUG  /* notice the , is with the m_string */
  , m_string("")
#endif
{
}

auto_parse::Word::Word(const Lexicon& l, const std::string& word)
  :
  m_index(l(word))
#ifndef NDEBUG  /* notice the , is with the m_string */
  ,m_string(word)
#endif
{
}

auto_parse::Word::Word(const auto_parse::Word& other)
  :
  m_index(other.m_index)
#ifndef NDEBUG  /* notice the , is with the m_string */
  ,m_string(other.m_string)
#endif
{
}

auto_parse::Word&
auto_parse::Word::operator=(const auto_parse::Word& other) 
{
  m_index = other.m_index;
#ifndef NDEBUG
  m_string = other.m_string;
#endif
  return *this;
}

void
auto_parse::Word::print_on(const Lexicon&
#ifdef NDEBUG  /* only used in this case */
    l
#endif
    , std::ostream& s)  const
{
#ifndef NDEBUG
  s << m_string;
#else
  s << l(m_index);
#endif
}

std::string
auto_parse::Word::convert_to_string(const Lexicon&
#ifdef NDEBUG  /* only used in this case */
    l
#endif
    )  const
{
#ifndef NDEBUG
  return m_string;
#else
  return(l(m_index));
#endif
}


auto_parse::Words::Words(const Lexicon* pl)
  :
  mp_l(pl),
   m_words()
{
}

auto_parse::Words::Words(const Lexicon& l,const std::string& w )
  :
  mp_l(&l),
   m_words()
{
  m_words.push_back(Word(l,w));
}



auto_parse::Words
auto_parse::reverse(const Words& w)
{
  Words result(w.p_lexicon());
  for(auto i = w.rbegin(); i != w.rend();++i)
    result.push_back(*i);
  return result;
}
std::vector<auto_parse::Words>
auto_parse::reverse(const std::vector<Words>& sentences)
{
  std::vector<Words> result;
  for(auto w : sentences)
    result.push_back(reverse(w));
  return result;
}

auto_parse::Words
operator+(const auto_parse::Words& W, std::string w)
{
  auto_parse::Words result = W;
  result.push_back(auto_parse::Word(*W.p_lexicon(),w));
  return(result);
}



bool
auto_parse::Words::operator==(const auto_parse::Words& other) const
{
  assert(mp_l == other.mp_l);
  if(m_words.size() != other.m_words.size())
    return false;
  for(unsigned int i = 0; i < m_words.size(); ++i)
    if(m_words[i].as_index() != other.m_words[i].as_index())
      return false;
  return true;
}

std::ostream&
operator<<(std::ostream & ostrm,const auto_parse::Words & sentence)
{
  for(auto_parse::Word w: sentence)
    {
      w.print_on(*sentence.p_lexicon(),ostrm);
      ostrm << " , ";
    }
  return ostrm;
}

// -*- c++ -*-

#include "word.h"
#include <iomanip>
#include <iterator>
#include <assert.h>

auto_parse::Lexicon::Lexicon(const std::set<std::string>& indexes)
  :
  m_index(),
  m_words(indexes.size())
{
  for(auto i = begin(indexes), e = end(indexes); i != e; ++i)
    m_index.insert(m_index.end(),std::make_pair(*i,m_index.size()));
  assert(m_index.find("<OOV>") != m_index.end());
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
  m_words(m_index.size())
{
  assert(m_index.find("<OOV>") != m_index.end());
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
    location = m_index.find("<OOV>");
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
  m_word()
{
}

auto_parse::Word::Word(const std::string& word)
  :
  m_word(word)
{
}

auto_parse::Word::Word(const auto_parse::Word& other)
  :
  m_word(other.m_word)
{
}

auto_parse::Word&
auto_parse::Word::operator=(const auto_parse::Word& other) 
{
  m_word = other.m_word;
  return *this;
}

void
auto_parse::Word::print_on(std::ostream& s)  const
{
  s << convert_to_string();
}

std::string
auto_parse::Word::convert_to_string()  const
{
  return(m_word);
}


auto_parse::Words
auto_parse::reverse(const Words& w)
{
  Words result;
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
  result.push_back(auto_parse::Word(w));
  return(result);
}


std::ostream&
operator<<(std::ostream & ostrm,const auto_parse::Words & sentence)
{
  for(auto_parse::Word w: sentence)
    {
      w.print_on(ostrm);
      ostrm << " , ";
    }
  return ostrm;
}

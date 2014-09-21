// -*- c++ -*-

#include "word.h"
#include <iomanip>
#include <iterator>



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

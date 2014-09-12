// -*- c++ -*-

#include "word.h"
#include <iomanip>
#include <iterator>

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
operator+(const auto_parse::Words& W, auto_parse::Word w)
{
  auto_parse::Words result = W;
  result.push_back(w);
  return(result);
}


std::ostream&
operator<<(std::ostream & ostrm,const auto_parse::Words & w)
{
  std::copy(w.begin(), w.end(), std::ostream_iterator<std::string>(ostrm," , "));
  return ostrm;
}

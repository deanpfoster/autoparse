// -*- c++ -*-

#include "word.h"
#include <iomanip>
#include <iterator>

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

// -*- c++ -*-

#include "decorated_dependency.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <map>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Decorated_dependency::~Decorated_dependency()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Decorated_dependency::Decorated_dependency(const Dependency&d, const Eigenwords& dict)
  : Dependency(d),
    m_dictionary(dict),
    m_description(),
    m_root()
{
};

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

void
auto_parse::Decorated_dependency::describe_link(const Link& link, const std::string& s)
{
  m_description[link] = s;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Decorated_dependency::describe_root(const std::string& s)
{
  m_root = s;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Decorated_dependency::describe_link(const Link& link, double d)
{
  d = round(d * 100) / 100;
  std::stringstream s;
  s << d;
  m_description[link] = s.str();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Decorated_dependency::describe_root(double d)
{
  d = round(d * 100) / 100;
  std::stringstream s;
  s << d;
  m_root = s.str();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors

std::string
auto_parse::Decorated_dependency:: link_description(const Link& link) const
{
  if(m_description.find(link) != m_description.end())
    return(m_description.find(link)->second);
  else
    return("??");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Decorated_dependency:: word_description(const Word &w) const
{
  std::string result = w;

  if(m_dictionary.find(w) == m_dictionary.end())
    result += ".OOV";
  return(result);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Decorated_dependency:: root_description() const
{
  return(m_root);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// -*- c++ -*-


#include "history.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <iterator>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::History::~History()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::History::History()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::History::History(const History & other)
  :
  m_actions(other.m_actions)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
void
auto_parse::History::push_back(Action a)
{
  m_actions.push_back(a);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::History::pop_back()
{
  m_actions.pop_back();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
bool
auto_parse::History::operator==(const History& other) const
{
  if(other.m_actions.size() != m_actions.size())
    return false;
  for(unsigned int i = 0; i < m_actions.size(); ++i)
    if(other[i] != (*this)[i])
      return false;
  return true;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::History::print_on(std::ostream & ostrm) const
{
  for(auto i = m_actions.begin(); i != m_actions.end(); ++i)
    {
      ostrm << *i << " ";
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Action
auto_parse::History::last() const
{
  return *m_actions.rbegin();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::History::size() const
{
  return static_cast<int>(m_actions.size());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int stack_increment(auto_parse::Action a)
{
  switch(a)
    {
    case auto_parse::Action::shift       : return +1;
    case auto_parse::Action::left_reduce : return -1;
    case auto_parse::Action::right_reduce: return -1;
    case auto_parse::Action::left_eager  : return -1;
    case auto_parse::Action::right_eager : return +1;
    case auto_parse::Action::reduce      : return -1;
    case auto_parse::Action::head_reduce : return -1;
    default :
      assert(0);
    }
  return 0;  // shouldn't be reached
}

int
auto_parse::History::maximum_stack_size() const
{
  int result = 0;
  int stack_size = 0;
  for(Action a : m_actions)
    {
      stack_size += stack_increment(a);
      if(stack_size > result)
	result = stack_size;
    };
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


std::ostream&
operator<<(std::ostream& os, auto_parse::Action a)
{
  switch(a)
    {
    case auto_parse::Action::shift       : os << "shift"; break;
    case auto_parse::Action::left_reduce : os << "<--"; break;
    case auto_parse::Action::right_reduce: os << "-->"; break;
    case auto_parse::Action::reduce      : os << "reduce"; break;
    case auto_parse::Action::left_eager  : os << "<-E-"; break;
    case auto_parse::Action::right_eager : os << "-E->"; break;
    case auto_parse::Action::head_reduce : os << "HEAD"; break;
    default :
      int i = static_cast<int>(a);
      if(i < 0)
	assert(0);
      os << "-" << i << "->";
    }
  return os;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::istream&
operator>>(std::istream& in, auto_parse::Action & a)
{
  std::string name;
  in >> name >> std::ws;
  if(name == "shift")
    a = auto_parse::Action::shift;
  else if(name == "<--")
    a = auto_parse::Action::left_reduce;
  else if(name == "<-E-")
    a = auto_parse::Action::left_eager;
  else if(name == "-->")
    a = auto_parse::Action::right_reduce;
  else if(name == "-E->")
    a = auto_parse::Action::right_eager;
  else if(name == "reduce")
    a = auto_parse::Action::reduce;
  else if(name == "HEAD")
    a = auto_parse::Action::head_reduce;
  else if(name == "-2->")
    a = auto_parse::Action::right_2;
  else if(name == "-3->")
    a = auto_parse::Action::right_3;
  else if(name == "-4->")
    a = auto_parse::Action::right_4;
  else if(name == "-5->")
    a = auto_parse::Action::right_5;
  else
    {
      std::cerr << "oops, can't read `" << name << "' as an action." << std::endl;
      assert(0);
    }
  return in;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


std::ostream & operator<<(std::ostream & ostrm, const auto_parse::History & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

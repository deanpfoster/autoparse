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
auto_parse::History::push_back(Actions a)
{
  m_actions.push_back(a);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
void
auto_parse::History::print_on(std::ostream & ostrm) const
{
  for(auto i = m_actions.begin(); i != m_actions.end(); ++i)
    {
      ostrm << *i << " ";
    }
};
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
operator<<(std::ostream& os, auto_parse::Actions a)
{
  switch(a)
    {
    case auto_parse::Actions::shift       : os << "shift"; break;
    case auto_parse::Actions::left_reduce : os << "<--"; break;
    case auto_parse::Actions::right_reduce: os << "-->"; break;
    case auto_parse::Actions::head_reduce : os << "HEAD"; break;
    default :
      int i = static_cast<int>(a);
      if(i < 0)
	assert(0);
      os << "-" << i << "->";
    }
  return os;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


std::ostream & operator<<(std::ostream & ostrm, const auto_parse::History & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

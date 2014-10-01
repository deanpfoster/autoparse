// -*- c++ -*-


#include "interval.h"

#include "assert.h"
#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Interval::Interval(int left, int right)
  :
  m_left(left),
  m_right(right)
{
  assert(0 <= left);
  assert(left < right);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Intervals::Intervals(const Dependency& d)
  :
  m_intervals()
{
  auto left_end = d.sentence().begin();
  for(auto l: d.links())
    if(l.parent() < l.child())
      m_intervals.push_back(Interval(l.parent() - left_end, l.child() - left_end));
    else
      m_intervals.push_back(Interval(l.child() - left_end, l.parent()- left_end));
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
bool
auto_parse::Interval::cross(const Interval& other) const
{ // us=Uu  other=Oo
  if(other.m_right <= m_left)
    return false;   //  Uu Oo
  if(m_right <= other.m_left)
    return false;   // Oo Uu
  // they have to have some overlap.  But are they nested?
  if(m_left < other.m_left) // OU
    return(m_right < other.m_right);  // true=OUou   false=OUuo
  else // UO
    if(m_left == other.m_left)
      return(false); // 
    else
      return(other.m_right < m_right);  // true = UOuo   false=UOou
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Interval::cross_check(const Interval& other) const
{
  std::stringstream s;
  s << "[" << m_left << "," << m_right << ")";
  if(cross(other))
    s << " crosses ";
  else
    s << " doesn't cross ";
  s << "[" << other.m_left << "," << other.m_right << ")" << std::endl;
  return s.str();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Intervals::count_crosses(const Intervals& other) const
{
  int result = 0;
  for(auto a : m_intervals)
    for(auto b : other.m_intervals)
      {

	result += a.cross(b);
      }
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
//                           P R I V A T E                                           private
//                     F R E E   F U N C T I O N S                            free functions

// -*- c++ -*-


#include "lr.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using


////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::LR::~LR()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::LR::LR(const Words& w)
  :     m_parse(w),
	m_stack(),
	m_next_input(m_parse.sentence().begin())
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::LR::LR(const LR & other)
  :
  m_parse(other.m_parse),
  m_stack(),
  m_next_input(other.m_next_input)
{
  for(auto i = other.m_stack.begin(); i != other.m_stack.end(); ++i)
    m_stack.push_back(*i - other.m_parse.sentence().begin() + m_parse.sentence().begin());
  m_next_input = other.m_next_input - other.m_parse.sentence().begin() + m_parse.sentence().begin();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
void
auto_parse::LR::shift()
{
  assert(m_next_input != m_parse.sentence().end()); // can't shift if we are already at the end
  m_stack.push_back(m_next_input);
  ++m_next_input;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::head_reduce()
{
  assert(!m_stack.empty());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
  assert(m_stack.empty());
  m_parse.set_root(right - m_parse.sentence().begin());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::left_reduce()
{
  assert(!m_stack.empty());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
  assert(!m_stack.empty());
  Node left = *m_stack.rbegin();
  m_stack.pop_back();
  m_parse.add(left - m_parse.sentence().begin(),Left_arrow(),right - m_parse.sentence().begin());
  m_stack.push_back(right);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::right_reduce()
{
  assert(!m_stack.empty());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
  assert(!m_stack.empty());
  Node left = *m_stack.rbegin();
  m_parse.add(left - m_parse.sentence().begin(), Right_arrow(), right - m_parse.sentence().begin());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::right_cross_reduce(int skip)
{
  assert(m_stack.size() >= static_cast<unsigned int>(skip)+1);
  Node top = *m_stack.rbegin();
  Node deep = *(m_stack.rbegin() + skip);
  m_parse.add(deep - m_parse.sentence().begin(), Right_arrow(), top - m_parse.sentence().begin());
  m_stack.pop_back();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::take_action(Action a)
{
  switch(a)
    {
    case auto_parse::Action::shift       : shift(); break;
    case auto_parse::Action::left_reduce : left_reduce(); break;
    case auto_parse::Action::right_reduce: right_reduce(); break;
    case auto_parse::Action::head_reduce : head_reduce(); break;
    default :
            int i = static_cast<int>(a);
	    if(i < 0)
	      assert(0);
	    right_cross_reduce(i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Word
auto_parse::LR::next_input() const
{
  return *m_next_input;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Node
auto_parse::LR::stack_top() const
{
  return *m_stack.rbegin();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Node
auto_parse::LR::stack(int depth) const
{
  assert(m_stack.size() >= static_cast<unsigned int>(depth)+1);
  return *(m_stack.rbegin() + depth);;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
const auto_parse::Dependency&
auto_parse::LR::parse() const
{
  //  assert(m_parse.full_parse());
  return m_parse;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::print_on(std::ostream & ostrm) const
{
  ostrm << "\nStack: ";
  for(Node n : m_stack)
    ostrm << *n << " ";
  ostrm << "\n" << m_parse;  
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

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::LR & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

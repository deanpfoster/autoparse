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
  : m_sentence(w),
    m_stack(),
    m_next_input(m_sentence.begin()),
    m_parse(w)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::LR::LR(const LR & other)
  :
  m_sentence(other.m_sentence),
  m_stack(other.m_stack),
  m_next_input(other.m_next_input),
  m_parse(other.m_parse)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
void
auto_parse::LR::shift()
{
  assert(m_next_input != m_sentence.end()); // can't shift if we are already at the end
  m_stack.push(m_next_input);
  ++m_next_input;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::head_reduce()
{
  Node right = m_stack.top();
  m_stack.pop();
  assert(m_stack.empty());
  m_parse.set_root(right - m_sentence.begin());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::left_reduce()
{
  Node right = m_stack.top();
  m_stack.pop();
  assert(!m_stack.empty());
  Node left = m_stack.top();
  m_stack.pop();
  m_parse.add(left - m_sentence.begin(),Left_arrow(),right - m_sentence.begin());
  m_stack.push(right);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::right_reduce()
{
  Node right = m_stack.top();
  m_stack.pop();
  assert(!m_stack.empty());
  Node left = m_stack.top();
  m_parse.add(left - m_sentence.begin(), Right_arrow(), right - m_sentence.begin());
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
  return m_stack.top();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Node
auto_parse::LR::stack_2() const
{
  // This isn't a natural operation. It should be: tmp=pop;result=top;push(tmp);
  assert(m_stack.size() >= 2);
  std::stack<Node>& evil_stack = const_cast<std::stack<Node>&>(m_stack);
  auto_parse::Node tmp = evil_stack.top();
  evil_stack.pop();
  auto_parse::Node result = evil_stack.top();
  evil_stack.push(tmp);
  return result;
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
auto_parse::LR::print_on(std::ostream & ) const
{
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

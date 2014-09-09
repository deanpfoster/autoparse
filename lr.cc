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
  assert(check_shift());
  m_stack.push_back(m_next_input);
  ++m_next_input;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::shift_eager()
{
  assert(check_shift_eager());
  shift();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::head_reduce()
{
  assert(check_head_reduce());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
  m_parse.set_root(right - m_parse.sentence().begin());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::left_reduce()
{
  assert(check_left_reduce());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
  Node left = *m_stack.rbegin();
  m_stack.pop_back();
  m_parse.add(left - m_parse.sentence().begin(),Left_arrow(),right - m_parse.sentence().begin());
  m_stack.push_back(right);
  assert(left != right); 
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::left_eager()
{
  assert(check_left_eager());
  Node left = *m_stack.rbegin();
  m_stack.pop_back();
  Node right = m_next_input;
  m_parse.add(left - m_parse.sentence().begin(),Left_arrow(),right - m_parse.sentence().begin());
  m_stack.push_back(right);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::right_reduce()
{
  assert(check_right_reduce());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
  Node left = *m_stack.rbegin();
  m_parse.add(left - m_parse.sentence().begin(), Right_arrow(), right - m_parse.sentence().begin());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::LR::right_eager()
{
  assert(check_right_eager());
  Node right = *m_stack.rbegin();
  m_stack.pop_back();
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
  assert(top != deep);
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
    case auto_parse::Action::left_eager  : left_eager(); break;
    case auto_parse::Action::right_eager : right_eager(); break;
    default :
      int i = static_cast<int>(a) - static_cast<int>(Action::right_reduce) + 1;
      if(i < 0)
	assert(0);
      right_cross_reduce(i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Word
auto_parse::LR::next_word() const
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
int
auto_parse::LR::number_words_left() const
{
  return m_parse.sentence().end() - m_next_input;
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
bool
auto_parse::LR::legal(auto_parse::Action a) const
{
  switch(a)
    {
    case auto_parse::Action::shift       : return check_shift(); break;
    case auto_parse::Action::shift_eager : return check_shift_eager(); break;
    case auto_parse::Action::left_eager  : return check_left_eager(); break;
    case auto_parse::Action::right_eager : return check_right_eager(); break;
    case auto_parse::Action::left_reduce : return check_left_reduce(); break;
    case auto_parse::Action::right_reduce: return check_right_reduce(); break;
    case auto_parse::Action::head_reduce : return check_head_reduce(); break;
    default :
      int i = static_cast<int>(a) - static_cast<int>(Action::right_reduce) + 1;
      if(i < 0)
	assert(0);
      return check_right_cross_reduce(i);
    }

};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_shift() const
{
  return(m_next_input != m_parse.sentence().end());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_shift_eager() const
{
  if(m_parse.sentence().end() - m_next_input > 1)
    return true; // we still have something in the queue
  if(m_parse.sentence().end() == m_next_input)
    return false; // nothing in the queue to shift
  return(m_stack.empty());  // we will follow this by a head_reduce and be done.
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_left_eager() const
{
  if(m_stack.empty())
    return false;
  return(m_next_input != m_parse.sentence().end());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_right_eager() const
{
  if(m_stack.empty() || (m_next_input == m_parse.sentence().end()))
    return false; // simply can't be done
  // Ok, the action could work--but will it leave us in an impossible state?
  if(m_parse.sentence().end() - m_next_input > 1)
    return true; // we still have something in the queue, so the future is fine
  assert(m_parse.sentence().end() - m_next_input == 1);  // so the queue will be empty
  return(m_stack.size() == 1);  // we can follow this with a head_reduce and we are done
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_left_reduce() const
{
  return(m_stack.size() >= 2);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_right_reduce() const
{
  return(m_stack.size() >= 2);
} 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_head_reduce() const
{
  if(m_next_input != m_parse.sentence().end())
    return false;
  return(m_stack.size() == 1);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::LR::check_right_cross_reduce(int i) const
{
  assert(i >= 0);
  return(m_stack.size() >= static_cast<unsigned int>(i)+1);
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

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::LR & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

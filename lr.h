// -*- c++ -*-

#ifndef INCLUDED_LR
#define INCLUDED_LR

#include <iosfwd>
#include "dependency.h"
#include <stack>

namespace auto_parse
{
  class LR
  {
  public:
    // CONSTRUCTORS
    ~LR();
    LR(const Words&);

    // MANIPULATORS
    void shift();
    void left_reduce();
    void head_reduce();  // used only to pop last item off the stack and set it as the head
    void right_reduce();

    // ACCESSORS
    void print_on(std::ostream &) const;
    Word next_input() const;
    Node stack_top() const;
    Node stack_2() const;  // first item under the top
    bool empty() const; // even the S has been pop-ed off the stack
    const Dependency& parse() const;

  protected:
    
    
  private:
    Words m_sentence; // These are also stored in the m_parse.  This duplication is wrong!
    std::stack<Node> m_stack;  // this should be switched to a vector where we have reserved enough places to hold everything
    auto_parse::const_word_iterator m_next_input;
    Dependency m_parse;
    
    LR(const LR &);            // Don't delete this.
    LR& operator=(const LR &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::LR &);

#endif

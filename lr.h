// -*- c++ -*-

#ifndef INCLUDED_LR
#define INCLUDED_LR

#include <iosfwd>
#include "dependency.h"

namespace auto_parse
{
  class LR
  {
  public:
    // CONSTRUCTORS
    ~LR();
    LR(const Words&);

    LR* clone() const;
    // MANIPULATORS
    void shift();
    void reduce(const Node&);

    // ACCESSORS
    virtual void print_on(std::ostream &) const;
    Word next_input() const;
    Node top() const;
    Node top_2() const;  // first item under the top
    bool empty() const; // even the S has been pop-ed off the stack
    Dependency parse() const;

  protected:

  private:
    Words m_sentence;
    std::stack<Node> m_stack;
    const_word_iterator m_next_input;
    
    LR(const LR &);            // Don't delete this.
    LR& operator=(const LR &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::LR &);

#endif

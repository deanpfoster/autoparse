// -*- c++ -*-

#ifndef INCLUDED_LR
#define INCLUDED_LR

#include <iosfwd>
#include "dependency.h"
#include "history.h"
#include <vector>

namespace auto_parse
{
  class LR
  {
  public:
    // CONSTRUCTORS
    ~LR();
    LR(const Words&);
    LR(const LR &); 

    // MANIPULATORS
    void shift_eager();
    void left_eager();
    void right_eager();  
    void shift();
    void left_reduce();
    void head_reduce();  // used only to pop last item off the stack and set it as the head
    void right_reduce();
    void right_cross_reduce(int depth); // (depth=1) is the same as right_reduce
    void take_action(auto_parse::Action);

    // ACCESSORS
    void print_on(std::ostream &) const;
    Node next_word() const;
    Node next_next_word() const;
    Node stack_top() const;
    Node stack(int depth) const;  // ith item under top 0=stack_top, 1=second item, 2=third item, etc
    bool empty_input() const; // even the S has been pop-ed off the stack
    bool empty_stack() const; // Nothing in the stack
    const Dependency& parse() const;
    bool legal(auto_parse::Action) const;
    int  number_words_left() const;
    int  stack_size() const{return m_stack.size();};

  protected:
    
    
  private:
    bool check_shift_eager() const;
    bool check_left_eager() const;
    bool check_right_eager() const;  
    bool check_shift() const;
    bool check_left_reduce() const;
    bool check_right_reduce() const;
    bool check_head_reduce() const;
    bool check_right_cross_reduce(int) const;


    Dependency m_parse;
    std::vector<Node> m_stack; 
    auto_parse::const_word_iterator m_next_input;
    
    LR& operator=(const LR &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::LR &);

#endif

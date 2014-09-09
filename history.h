// -*- c++ -*-

#ifndef INCLUDED_HISTORY
#define INCLUDED_HISTORY

#include <iosfwd>
#include <vector>

namespace auto_parse
{
  enum class Action {shift, shift_eager, left_eager, right_eager, left_reduce, head_reduce, right_reduce, right_2, right_3 };
  const Action all_possible_actions[] = { Action::shift,
					Action::left_reduce, Action::right_reduce, Action::head_reduce,
					Action::right_2, Action::right_3};
  const std::vector<Action> eager_actions = {Action::shift_eager, Action::left_eager, Action::right_eager, Action::head_reduce};
  const std::vector<Action> standard_actions = { Action::shift, Action::left_reduce, Action::right_reduce, Action::head_reduce};
  const std::vector<Action> all_actions = standard_actions;
  
  class History
  {
  public:
    // CONSTRUCTORS
    virtual ~History();
    History();
    History(const History &);

    // MANIPULATORS
    void push_back(Action);
    void pop_back();
    // ACCESSORS
    Action operator[](int i) const{return m_actions[i];};
    std::vector<Action>::const_iterator begin() const {return m_actions.begin();};
    std::vector<Action>::const_iterator end() const {return m_actions.end();};
    bool operator==(const History&) const;
    virtual void print_on(std::ostream & ostrm) const;
    Action last() const;
    int size() const;
    int maximum_stack_size() const;
    
  protected:

  private:
    std::vector<Action> m_actions;
  };
}

std::ostream& operator<<(std::ostream & , auto_parse::Action);
std::istream& operator>>(std::istream & , auto_parse::Action&);
std::ostream& operator<<(std::ostream & , const auto_parse::History &);

#endif

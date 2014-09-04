// -*- c++ -*-

#ifndef INCLUDED_HISTORY
#define INCLUDED_HISTORY

#include <iosfwd>
#include <vector>

namespace auto_parse
{
  enum class Action {shift=-5, left_reduce=-1, head_reduce=0, right_reduce=1, right_2=2, right_3, right_4, right_5 };
  const Action all_legal_actions[] = { Action::shift,
					Action::left_reduce, Action::right_reduce, Action::head_reduce,
					Action::right_2, Action::right_3, Action::right_4, Action::right_5};
  const std::vector<Action> all_actions = { Action::shift,
				  Action::left_reduce, Action::right_reduce, Action::head_reduce};
  
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

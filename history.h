// -*- c++ -*-

#ifndef INCLUDED_HISTORY
#define INCLUDED_HISTORY

#include <iosfwd>
#include <vector>

namespace auto_parse
{
  enum class Action {shift=-5, left_reduce, head_reduce, right_reduce, right_2=2, right_3, right_4, right_5 };
  const Action All_legal_actions[] = { Action::shift,
					Action::left_reduce, Action::right_reduce, Action::head_reduce,
					Action::right_2, Action::right_3, Action::right_4, Action::right_5};
  const Action All_actions[] = { Action::shift,
				  Action::left_reduce, Action::right_reduce, Action::head_reduce};
  

  class History
  {
  public:
    // CONSTRUCTORS
    ~History();
    History();

    // MANIPULATORS
    void push_back(Action);
    // ACCESSORS
    std::vector<Action>::const_iterator begin() const {return m_actions.begin();};
    std::vector<Action>::const_iterator end() const {return m_actions.end();};
    virtual void print_on(std::ostream & ostrm) const;
    
  protected:

  private:
    std::vector<Action> m_actions;
    History(const History &);            // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream & , auto_parse::Action);
std::ostream& operator<<(std::ostream & , const auto_parse::History &);

#endif

// -*- c++ -*-

#ifndef INCLUDED_HISTORY
#define INCLUDED_HISTORY

#include <iosfwd>
#include <vector>

namespace auto_parse
{
  enum class Actions {shift, left_reduce, right_reduce, head_reduce};

  class History
  {
  public:
    // CONSTRUCTORS
    ~History();
    History();

    // MANIPULATORS
    void push_back(Actions);
    // ACCESSORS
    std::vector<Actions>::const_iterator begin() const {return m_actions.begin();};
    std::vector<Actions>::const_iterator end() const {return m_actions.end();};
    virtual void print_on(std::ostream & ostrm) const;
    
  protected:

  private:
    std::vector<Actions> m_actions;
    History(const History &);            // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream & , auto_parse::Actions);
std::ostream& operator<<(std::ostream & , const auto_parse::History &);

#endif

// -*- c++ -*-

#ifndef INCLUDED_DEPENDENCY
#define INCLUDED_DEPENDENCY

#include <iosfwd>
#include <vector>
#include <string>

namespace auto_parse
{
  class Dependency
  {
  public:
    typedef int Node;
    typedef std::vector<std::string> Words;
    typedef std::vector<std::pair<Node, Node> > Links;
    typedef Words::const_iterator const_word_iterator;
    typedef Links::const_iterator const_link_iterator;

    // CONSTRUCTORS
    ~Dependency();
    Dependency();

    Dependency* clone() const;
    // MANIPULATORS

    // ACCESSORS
    virtual Node root() const;
    virtual void latex(std::ostream &) const;
    virtual void print_on(std::ostream &) const;

  protected:

  private:
    Words m_words;
    Node  m_root;
    Links m_links;

    Dependency(const Dependency &);            // Don't delete this.
    Dependency& operator=(const Dependency &); // Don't delete this.
  };
};

std::ostream& operator<<(std::ostream &,const auto_parse::Dependency &);

#endif

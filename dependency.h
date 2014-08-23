// -*- c++ -*-

#ifndef INCLUDED_DEPENDENCY
#define INCLUDED_DEPENDENCY

#include <iosfwd>
#include <vector>
#include "word.h"

namespace auto_parse
{
  class Right_arrow{};
  class Left_arrow{};

  typedef std::pair<Node,Node>  Link;  // Node is a pointer to a Word
  typedef std::vector<Link> Links;

  class Dependency
  {
  public:
    typedef Links::const_iterator const_link_iterator;

    // CONSTRUCTORS
    ~Dependency();
    Dependency(const Word&); // generates most trival parse possible
    Dependency(const Words&); // generates an empty parse
    Dependency(const Dependency& left, Right_arrow, const Dependency& right);  // Head is left.head, old right.head comes from left now
    Dependency(const Dependency& left, Left_arrow, const Dependency& right);
    Dependency(const Dependency &);            // Don't delete this.

    // MANIPULATORS
    void set_root(int);
    void add(int left, Left_arrow ,  int right);
    void add(int left, Right_arrow , int right);

    // ACCESSORS
    Node root() const;
    void latex(std::ostream &) const;
    void print_on(std::ostream &) const;
    std::string link_description(const Link&) const;
    bool full_parse() const;
    const Links& links() const;
    const Words& sentence() const{return m_words;}

  protected:
    void set_root(const Node&); // if you can't see our internal data structure, you probably shouldn't be using these
    void add(const Node& left, Left_arrow ,  const Node& right);
    void add(const Node& left, Right_arrow , const Node& right);

  private:
    void add_words_and_links(const Dependency & left, const Dependency& right);

    Words m_words;
    Node  m_root;
    Links m_links;
    mutable bool  m_full_parse;

    Dependency& operator=(const Dependency &); // Don't delete this.
  };


};

std::ostream& operator<<(std::ostream &,const auto_parse::Dependency &);

// The following make for pretty parsing by hand:
auto_parse::Dependency
operator<(const auto_parse::Dependency& l, const auto_parse::Dependency& r);

auto_parse::Dependency
operator>(const auto_parse::Dependency& l, const auto_parse::Dependency& r);


#endif

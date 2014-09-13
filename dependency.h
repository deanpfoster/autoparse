// -*- c++ -*-

#ifndef INCLUDED_DEPENDENCY
#define INCLUDED_DEPENDENCY

#include <iosfwd>
#include <vector>
#include <set>
#include "word.h"

namespace auto_parse
{
  class Right_arrow{};
  class Left_arrow{};

  //  typedef std::pair<Node,Node>  Link;  // make_pair(a,b) == b depends on a, i.e. a --> b
  class Link
  {
  public:
    Link(const Node& parent, const Node& child)
      :
      m_parent(parent),
      m_child(child){};

    //accessor
    Node parent() const {return m_parent;};
    Node child() const {return m_child;};
    bool operator<(const Link& rhs) const{return std::make_pair(m_parent,m_child) < std::make_pair(rhs.m_parent,rhs.m_child);};

  private:
    Node m_parent;
    Node m_child;
  };

  typedef std::set<Link> Links;

  class Dependency
  {
  public:
    typedef Links::const_iterator const_link_iterator;

    // CONSTRUCTORS
    virtual ~Dependency();
    Dependency(const Word&); // generates most trival parse possible
    Dependency(const Words&); // generates an empty parse
    Dependency(const Dependency& left, Right_arrow, const Dependency& right);  // Head is left.head, old right.head comes from left now
    Dependency(const Dependency& left, Left_arrow, const Dependency& right);
    Dependency(const Dependency &);            // Don't delete this.

    // MANIPULATORS
    // The following should take a "Node" -- but I had problems of which
    // sentence it was working with.  So I switched to an evil integer.
    void set_root(int);
    void add(int left, Left_arrow ,  int right);
    void add(int left, Right_arrow , int right);

    // ACCESSORS
    Node root() const;
    void latex(std::ostream &) const;
    void latex_reversed(std::ostream &) const;
    void print_on(std::ostream &) const;
    bool full_parse() const;
    const Links& links() const;
    bool has_parent(const Node&) const;
    Node left_most_child(const Node&) const;  // returns end() on error
    Node right_most_child(const Node&) const;
    const Words& sentence() const{return m_words;}
    double number_left_links() const;

  protected:
    virtual std::string link_description(const Link&) const;
    virtual std::string word_description(const Word&) const;
    virtual std::string root_description() const;
    void set_root(const Node&); // if you can't see our internal data structure, you probably shouldn't be using these
    void add(const Node& left, Left_arrow ,  const Node& right);
    void add(const Node& left, Right_arrow , const Node& right);

  private:
    void add_words_and_links(const Dependency & left, const Dependency& right);

    Words m_words;
    Node  m_root;
    Links m_links; 
    std::vector<int> m_parent;
    mutable bool  m_full_parse;

    Dependency& operator=(const Dependency &); // Don't delete this.
  };

  void latex_header(std::ostream&,std::string = "runningTitle");
  void latex_footer(std::ostream&);
};

std::ostream& operator<<(std::ostream &,const auto_parse::Dependency &);

// The following make for pretty parsing by hand:
auto_parse::Dependency
operator<(const auto_parse::Dependency& l, const auto_parse::Dependency& r);

auto_parse::Dependency
operator>(const auto_parse::Dependency& l, const auto_parse::Dependency& r);


#endif

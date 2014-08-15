// -*- c++ -*-

#ifndef INCLUDED_DEPENDENCY
#define INCLUDED_DEPENDENCY

#include <iosfwd>
#include <vector>
#include <string>

namespace auto_parse
{
  class Right_arrow{};
  class Left_arrow{};

  typedef std::string Word;
  typedef std::vector<Word> Words;
  typedef Words::const_iterator Node; 
  typedef std::pair<Node,Node> Link;

  class Dependency
  {
  public:
    typedef std::vector<Link> Links;
    typedef Words::const_iterator const_word_iterator;
    typedef Links::const_iterator const_link_iterator;

    // CONSTRUCTORS
    ~Dependency();
    Dependency(const Word&); // generates most trival parse possible
    Dependency(const Dependency& left, Right_arrow, const Dependency& right);  // Head is left.head, old right.head comes from left now
    Dependency(const Dependency& left, Left_arrow, const Dependency& right);
    Dependency(const Dependency &);            // Don't delete this.

    Dependency* clone() const;
    // MANIPULATORS

    // ACCESSORS
    virtual Node root() const;
    virtual void latex(std::ostream &) const;
    virtual void print_on(std::ostream &) const;
    virtual std::string link_description(const Link&) const;

  protected:

  private:
    void add_words_and_links(const Dependency & left, const Dependency& right);

    Words m_words;
    Node  m_root;
    Links m_links;

    Dependency& operator=(const Dependency &); // Don't delete this.
  };
};

std::ostream& operator<<(std::ostream &,const auto_parse::Dependency &);

auto_parse::Dependency
operator<(const auto_parse::Dependency& l, const auto_parse::Dependency& r);

auto_parse::Dependency
operator>(const auto_parse::Dependency& l, const auto_parse::Dependency& r);

#endif

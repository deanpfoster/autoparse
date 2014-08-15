// -*- c++ -*-

#include "dependency.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <iterator>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

// using thing::Thing;  // example.  Try not to do: "using foo;"  It adds too much

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Dependency::~Dependency()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Word& w)
  : m_words(),
    m_root(),
    m_links()
{
  m_words.push_back(w);
  m_root = m_words.begin();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & other)
  : m_words(other.m_words),
    m_root(other.m_root),
    m_links(other.m_links)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & left, Right_arrow, const Dependency& right)
  : m_words(),
    m_root(),
    m_links()
{
  add_words_and_links(left,right);
  Node left_root = (left.m_root - left.m_words.begin()) + m_words.begin();
  Node right_offset = m_words.begin() + left.m_words.size();
  Node right_root = (right.m_root - right.m_words.begin()) + right_offset;
  m_links.push_back(std::make_pair(left_root,right_root));
  m_root = left_root;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & left, Left_arrow, const Dependency& right)
  : m_words(),
    m_root(),
    m_links()
{
  add_words_and_links(left,right);
  Node left_root = (left.m_root - left.m_words.begin()) + m_words.begin();
  Node right_offset = m_words.begin() + left.m_words.size();
  Node right_root = (right.m_root - right.m_words.begin()) + right_offset;
  m_links.push_back(std::make_pair(right_root,left_root));
  m_root = right_root;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency*
auto_parse::Dependency::clone() const
{
  return new Dependency(*this);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

auto_parse::Dependency&
auto_parse::Dependency::operator=(const auto_parse::Dependency & rhs)
{
  m_words = rhs.m_words;
  m_root  = rhs.m_root;
  m_links = rhs.m_links;
  // add code here
  return *this; 
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Node
auto_parse::Dependency::root() const
{
  return m_root;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::latex(std::ostream & ostrm) const
{
  ostrm << "\t\t%\t Include in header: \\usepackage{tikz-dependency}" << std::endl;
  ostrm << "\\begin{dependency}[theme = simple]" << std::endl;
  ostrm << "\\begin{deptext}[column sep=1em]" << std::endl;
  ostrm << "     ";
  for(const_word_iterator i = m_words.begin(); i != m_words.end();++i)
    {
      ostrm << *i ;
      const_word_iterator next = i;
      ++next;
      if(next != m_words.end())
	ostrm << " \\& ";
    };
  ostrm << "  \\\\" << std::endl;
  ostrm << "\\end{deptext}" << std::endl;
  int root_index = root() - m_words.begin() + 1;
  ostrm << "\\deproot{" << root_index << "}{ROOT}" << std::endl;
  for(const_link_iterator i = m_links.begin(); i != m_links.end();++i)
    {
      int from_index = i->first - m_words.begin() + 1;  // Latex uses 1 based indexing
      int to_index = i->second - m_words.begin() + 1;
      ostrm << "\\depedge{" << from_index << "}{" << to_index << "}{" << link_description(*i) << "}" << std::endl;
    }
  ostrm << "\\end{dependency}" << std::endl;
  ostrm << std::endl;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::string
auto_parse::Dependency:: link_description(const Link& l) const
{
  // when we end up with probabilities--these should be stuffed in here
  return("");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Dependency::print_on(std::ostream & ostrm) const
{
  latex(ostrm);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
void
auto_parse::Dependency::add_words_and_links(const Dependency & left, const Dependency& right)
{
  assert(m_words.size() == 0);
  assert(m_links.size() == 0);
  
  m_words = left.m_words;
  std::copy(right.m_words.begin(),right.m_words.end(),std::back_inserter(m_words));
  Node left_offset = m_words.begin();
  Node right_offset = m_words.begin() + left.m_words.size();
  for(const_link_iterator i = left.m_links.begin(); i != left.m_links.end();++i)
    {
      Node start = i -> first;
      Node end   = i -> second;
      int start_index = start - left.m_words.begin();
      int end_index = end - left.m_words.begin();
      m_links.push_back(std::make_pair(start_index+left_offset, end_index+left_offset));
    }
  for(const_link_iterator i = right.m_links.begin(); i != right.m_links.end();++i)
    {
      Node start = i -> first;
      Node end   = i -> second;
      int start_index = start - right.m_words.begin();
      int end_index = end - right.m_words.begin();
      m_links.push_back(std::make_pair(start_index+right_offset, end_index+right_offset));
    }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Dependency & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency
operator<(const auto_parse::Dependency& l, const auto_parse::Dependency& r)
{
  return (auto_parse::Dependency(l,auto_parse::Left_arrow(),r));
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency
operator>(const auto_parse::Dependency& l, const auto_parse::Dependency& r)
{
  return (auto_parse::Dependency(l,auto_parse::Right_arrow(),r));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


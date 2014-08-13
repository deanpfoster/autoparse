// -*- c++ -*-

#include "dependency.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

// using thing::Thing;  // example.  Try not to do: "using foo;"  It adds too much

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Dependency::~Dependency()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency()
  : m_words(),
    m_root(),
    m_links()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & other)
  : m_words(other.m_words),
    m_root(other.m_root),
    m_links(other.m_links)
{
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
  ostrm << "\\deproot{" << root() << "}{ROOT}" << std::endl;
  for(const_link_iterator i = m_links.begin(); i != m_links.end();++i)
    {
      ostrm << "\\depedge{" << i->first << "}{" << i->second << "}{prob?}" << std::endl;
    }
  ostrm << "\\end{dependency}" << std::endl;
  ostrm << std::endl;
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Dependency & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

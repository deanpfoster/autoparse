// -*- c++ -*-

#include "dependency.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <map>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

// using thing::Thing;  // example.  Try not to do: "using foo;"  It adds too much

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Dependency::~Dependency()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Lexicon& l, const Word& w)
  : m_words(l,w.p_lexicon()),
    m_root(),
    m_links(),
    m_parent(1,-1),
    m_full_parse(true)
{
  m_words.push_back(w);
  m_root = m_words.begin();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Words& w)
  : m_words(w),
    m_root(m_words.end()),
    m_links(),
    m_parent(w.size(),-1),
    m_full_parse(false)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & other)
  : m_words(other.m_words),
    m_root(m_words.end()),
    m_links(),
    m_parent(other.m_parent),
    m_full_parse(other.m_full_parse)
{
  int shift = m_words.begin() - other.m_words.begin();
  if(other.m_root != other.m_words.end())
    m_root = other.m_root + shift;
  for(auto i = other.m_links.begin(); i != other.m_links.end(); ++i)
    {
      Node p = i->parent() + shift;
      Node c = i->child() + shift;
      Link l(p,c);
      m_links.insert(m_links.end(),l);
    }
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & left, Right_arrow, const Dependency& right)
  : m_words(),
    m_root(),
    m_links(),
    m_parent(),
    m_full_parse(true)
{
  assert(left.m_full_parse);
  assert(right.m_full_parse);
  add_words_and_links(left,right);
  Node left_root = (left.m_root - left.m_words.begin()) + m_words.begin();
  Node right_offset = m_words.begin() + left.m_words.size();
  Node right_root = (right.m_root - right.m_words.begin()) + right_offset;
  m_links.insert(Link(left_root,right_root));
  m_parent[right_root - m_words.begin()] = left_root - m_words.begin();
  m_root = left_root;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Dependency::Dependency(const Dependency & left, Left_arrow, const Dependency& right)
  : m_words(),
    m_root(),
    m_links(),
    m_full_parse(true)
{
  assert(left.m_full_parse);
  assert(right.m_full_parse);
  add_words_and_links(left,right);
  Node left_root = (left.m_root - left.m_words.begin()) + m_words.begin();
  Node right_offset = m_words.begin() + left.m_words.size();
  Node right_root = (right.m_root - right.m_words.begin()) + right_offset;
  m_links.insert(Link(right_root,left_root));
  m_parent[left_root - m_words.begin()] = right_root - m_words.begin();
  m_root = right_root;
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
  m_parent = rhs.m_parent;
  assert(0);
  // add code here
  return *this; 
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::set_root(const Node& root)
{
  assert(m_root == m_words.end());// confirm we haven't set it yet
  m_root = root;
  m_parent[m_root - m_words.begin()] = m_words.end() - m_words.begin();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::add(const auto_parse::Node& left,
			    auto_parse::Left_arrow ,
			    const auto_parse::Node& right)
{
  assert(right - left > 0); // make sure we got them in the right order
  assert(left - m_words.begin() >= 0);
  assert(m_words.end() - right > 0);
  assert(!full_parse());
  m_links.insert(Link(right,left));
  m_parent[left - m_words.begin()] = right - m_words.begin();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::add(const auto_parse::Node& left,
			    auto_parse::Right_arrow,
			    const auto_parse::Node& right)
{
  assert(right - left > 0); // make sure we got them in the right order
  assert(left - m_words.begin() >= 0);
  assert(m_words.end() - right > 0);
  assert(!full_parse());
  m_links.insert(Link(left,right));
  m_parent[right - m_words.begin()] = left - m_words.begin();
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::set_root(int root)
{
  assert(root < int(m_words.size()));
  assert(root >= 0);
  set_root(m_words.begin() + root);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::add(int left, auto_parse::Left_arrow , int right)
{
  assert(left >= 0);
  assert(right > left);
  assert(right < int(m_words.size()));
  add(m_words.begin() + left, Left_arrow(), m_words.begin() + right);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::add(int left, auto_parse::Right_arrow, int right)
{
  assert(right < int(m_words.size()));
  assert(left >= 0);
  assert(right > left);
  add(m_words.begin() + left, Right_arrow(), m_words.begin() + right);
}

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
  for(auto i = m_words.begin(); i != m_words.end();++i)
    {
      ostrm << word_description(*i) ;
      auto next = i;
      ++next;
      if(next != m_words.end())
	ostrm << " \\& ";
    };
  ostrm << "  \\\\" << std::endl;
  ostrm << "\\end{deptext}" << std::endl;
  int root_index = root() - m_words.begin() + 1;
  ostrm << "\\deproot{" << root_index << "}{ROOT " << root_description() << "}" << std::endl;
  for(const_link_iterator i = m_links.begin(); i != m_links.end();++i)
    {
      int from_index = i->parent() - m_words.begin() + 1;  // Latex uses 1 based indexing
      int to_index = i->child() - m_words.begin() + 1;
      ostrm << "\\depedge{" << from_index << "}{" << to_index << "}{" << link_description(*i) << "}" << std::endl;
    }
  ostrm << "\\end{dependency}" << std::endl;
  ostrm << std::endl;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Dependency::latex_reversed(std::ostream & ostrm) const
{
  ostrm << "\t\t%\t Include in header: \\usepackage{tikz-dependency}" << std::endl;
  ostrm << "\\begin{dependency}[theme = simple]" << std::endl;
  ostrm << "\\begin{deptext}[column sep=1em]" << std::endl;
  ostrm << "     ";
  for(auto i = m_words.rbegin(); i != m_words.rend();++i)
    {
      ostrm << word_description(*i) ;
      auto next = i;
      ++next;
      if(next != m_words.rend())
	ostrm << " \\& ";
    };
  ostrm << "  \\\\" << std::endl;
  ostrm << "\\end{deptext}" << std::endl;
  int root_index = m_words.end() - root();
  ostrm << "\\deproot{" << root_index << "}{ROOT " << root_description() << "}" << std::endl;
  for(const_link_iterator i = m_links.begin(); i != m_links.end();++i)
    {
      int from_index = m_words.end() - i->parent();  // Latex uses 1 based indexing
      int to_index = m_words.end() - i->child();
      ostrm << "\\depedge{" << from_index << "}{" << to_index << "}{" << link_description(*i) << "}" << std::endl;
    }
  ostrm << "\\end{dependency}" << std::endl;
  ostrm << std::endl;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::string
auto_parse::Dependency:: link_description(const Link& ) const
{
  // when we end up with probabilities--these should be stuffed in here
  return("");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Dependency:: word_description(const Word &w) const
{
  return(w.convert_to_string());
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::Dependency:: root_description() const
{
  return("");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Dependency::print_on(std::ostream & ostrm) const
{
  std::map<Node, Node> parent;
  std::multimap<Node, Node> children;
  for(const_link_iterator i = m_links.begin(); i != m_links.end(); ++i)
    {
      assert(parent.find(i->child()) == parent.end());
      assert(m_parent[i->child() - m_words.begin()] == i->parent() - m_words.begin());
      parent[i->child()] = i->parent();
      children.insert(std::make_pair(i->parent(),i->child()));
    }
  for(Word w : m_words)
    ostrm << word_description(w) << " ";
  ostrm << std::endl;
  if(!full_parse())
    {
      ostrm << "\t\t\t * * * * Incomplete parse structure:   " << m_links.size() << " / " << m_words.size() << " * * * *" << std::endl;
      // The following was used for debugging the incomplete parse
      //      ostrm << "\t\t\t parents size: " << parent.size() << std::endl;
      //      ostrm << "\t\t\t children size: " << children.size() << std::endl;
      //      for(auto i = parent.begin(); i != parent.end();++i)
      //	ostrm << i->first - m_words.begin() << " --> " << i->second - m_words.begin() << std::endl;
    }
  for(auto i = m_words.begin(); i != m_words.end(); ++i)
    {
      if(parent.find(i) != parent.end())
	ostrm << std::left << std::setw(5) << link_description(Link(parent.find(i)->second,i)) <<std::right;
      else
	if(root() == i)
	  ostrm << std::left << std::setw(5) << root_description() <<std::right;
	else
	  ostrm << "     ";

      //       foo    <---    bar     (foo --> A, B, C)
      ostrm << "\t" << std::setw(15) << word_description(*i);
      if(i == root())
	ostrm  << "  <---  " << std::left << std::setw(15) << "ROOT" << std::right;
      else
	{
	  if(parent.find(i) != parent.end())
	    ostrm << "  <---  " << std::left << std::setw(15) << word_description(*parent[i]) << std::right;
	  else
	    ostrm << "  <---  " << std::setw(15) << " " ;
	};
      auto start = children.lower_bound(i);
      auto end   = children.upper_bound(i);
      if(start != end)
	{  // We've got kids to deal with
	  std::stringstream left_kids,right_kids;
	  bool first_left = true;
	  bool first_right = true;
	  for(auto j = start; j != end; ++j)
	    {
	      if(j->second < i) // we are a left kid
		{
		  if(!first_left)
		    left_kids << ", ";
		  left_kids << j->second->convert_to_string();
		  first_left = false;
		}
	      if(j->second > i)
		{
		  if(!first_right)
		    right_kids << ", ";
		  else
		    right_kids << "   --->  ";
		  right_kids << j->second->convert_to_string();
		  first_right = false;
		}
	    }
	  if(!first_left)
	    left_kids << "  <---   ";
	  ostrm << std::setw(40) << left_kids.str() << i->convert_to_string() << right_kids.str();
	}
      else
	ostrm << std::setw(40) << " " << i->convert_to_string();
      ostrm << std::endl;
    }
  ostrm << std::endl;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool
auto_parse::Dependency::full_parse() const
{
  if(!m_full_parse)
    {
      // Bummer--we ahve to compute it ourselves
      if(m_root == Node())
	return false; // root isn't set yet
      std::vector<bool> is_pointed_to(m_words.size());
      is_pointed_to[m_root - m_words.begin()] = true;
      for(const_link_iterator i = m_links.begin(); i != m_links.end(); ++i)
	{
	  //	  int from_index = i->first - m_words.begin();
	  int to_index = i->child() - m_words.begin();
	  assert(!is_pointed_to[to_index]);  // might as well check double pointers
	  is_pointed_to[to_index] = true;
	}
      m_full_parse = true;
      for(std::vector<bool>::const_iterator i = is_pointed_to.begin(); i != is_pointed_to.end();++i)
	if(!*i)
	  m_full_parse = false;
    }
  return(m_full_parse);

};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
const auto_parse::Links&
auto_parse::Dependency::links() const
{
  return m_links;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::Dependency::has_parent(const auto_parse::Node& location) const
{
  return(m_parent[location-m_words.begin()] != -1);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Node
auto_parse::Dependency::left_most_child(const auto_parse::Node& location) const
{
  if(m_links.size() == 0)
    return m_words.end(); 
  Link target(location,m_words.begin());
  auto i = m_links.lower_bound(target);
  if(i == m_links.end())
    return m_words.end();
  if((i->parent() == location) && (i->child() < location))
    // we found a link which starts with location and ends to the left.  Its a go!
    return i->child();
  return m_words.end();  // nothing useful found.
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Node
auto_parse::Dependency::right_most_child(const auto_parse::Node& location) const
{
  auto i = m_links.upper_bound(Link(location,m_words.end()));
  if(i == m_links.begin())
    return m_words.end();
  --i; // we now might be pointing at a link
  if((i->parent() == location) && (i->child() > location))
    // we found a link which starts with location and ends to the left.  Its a go!
    return i->child();
  return m_words.end();  // nothing useful found.
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
double
auto_parse::Dependency::number_left_links() const
{
  double result = 0.0;
  for(const_link_iterator i = m_links.begin(); i != m_links.end(); ++i)
    result += (i->parent() >  i->child());
  return result;
}
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
      Node start = i -> parent();
      Node end   = i -> child();
      int start_index = start - left.m_words.begin();
      int end_index = end - left.m_words.begin();
      m_links.insert(Link(start_index+left_offset, end_index+left_offset));
    }
  for(const_link_iterator i = right.m_links.begin(); i != right.m_links.end();++i)
    {
      Node start = i -> parent();
      Node end   = i -> child();
      int start_index = start - right.m_words.begin();
      int end_index = end - right.m_words.begin();
      m_links.insert(Link(start_index+right_offset, end_index+right_offset));
    }
  m_parent = std::vector<int>(m_words.size(), -1);
  for(auto link : m_links)
    m_parent[link.child() - m_words.begin()] = link.parent() - m_words.begin();
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
void
auto_parse::latex_header(std::ostream& out, std::string /* running_title */)
{
  out << "\\documentclass{article}\n\n";
  out << "\\usepackage[left=.25in,top=.25in,bottom=.25in,right=.25in]{geometry}\n";  
  out << "\\usepackage{tikz-dependency}\n\n";
  // I can't get the running header to work.  Oh well.
  //  out << "\\pagestyle{myheadings}\\markright{" << running_title << ": \\thepage}";
  out << "\\begin{document}\n\n" << std::endl;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::latex_footer(std::ostream& out)
{
  out << "\n\n\\end{document}" << std::endl;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// -*- c++ -*-


#include "conll_2006.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Conll_2006::~Conll_2006()
{
  m_input.close();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Conll_2006::Conll_2006(const std::string & name, const Lexicon* pl)
  :
  m_file_name(name),
  m_input(name),
  mp_lexicon(pl)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
auto_parse::Dependency
auto_parse::Conll_2006::next_sentence() 
{
  std::string one_line;
  getline(m_input,one_line);
  m_input >> std::ws; 
  std::map<int, int> parent_map;
  std::vector<std::string> word_vec;
  while(one_line != "")
    {
      std::stringstream s(one_line);
      int word_id;
      int parent_id;
      std::string raw_word, lc_word, POS, semantic_role, pos2, UK1, UK2, UK3, empty;
      s >> word_id >> raw_word >> lc_word >> UK1 >> POS >> semantic_rold >> parent_id >> pos2 >> UK2 >> UK3 >> std::ws;
      s >> empty;
      assert(empty == "");  // if not, we counted wrong!
      parent_map(word_id) = parent_id;
      word_vec.push_back(lc_word);
      getline(m_input,one_line);
    }
  // Now we have to convert it to a dependency parse
  Dependency result;
      std::stringstream s(one_line);
      Words result(mp_lexicon);
      while(!s.eof())
	{
	  std::string next_word;
	  s >> next_word;
	  boost::algorithm::to_lower(next_word);
	  if(next_word == "\"")
	    next_word = "QUOTE";
	  result.push_back(Word(*mp_lexicon,next_word));
	}
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Conll_2006::reset() 
{
  m_input.close();
  m_input.open(m_file_name);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::Conll_2006::eof() const
{
  return m_input.eof();
};

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


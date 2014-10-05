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
      s >> word_id >> raw_word >> lc_word >> UK1 >> POS >> semantic_role >> parent_id >> pos2 >> UK2 >> UK3 >> std::ws;
      s >> empty;
      assert(empty == "");  // if not, we counted wrong!
      parent_map[word_id] = parent_id;
      word_vec.push_back(lc_word);
      getline(m_input,one_line);
    }
  // Now we have to convert it to a dependency parse
  Words sentence(mp_lexicon);
  for(auto w:word_vec)
    sentence.push_back(w);
  Dependency result(sentence);
  for(auto p: parent_map)
    if(p.second == 0)
      result.set_root(p.first - 1);
    else
      result.child_to_its_parent(p.first - 1, p.second - 1);
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
void
auto_parse::Conll_2006::write_parse(const auto_parse::Dependency& d, std::ostream& out) const
{
  const Words& sentence = d.sentence();
  for(int i = 0; i < sentence.size(); ++i)
    {
      int word_id = i + 1;
      int parent_id = d.parents()[i] + 1;
      if(i == d.root() - d.sentence().begin())
	parent_id = 0; // this actually is magiclly true since we use -1 for root as the parent
      std::string raw_word = sentence[i].convert_to_string(*mp_lexicon);
      std::string lc_word = raw_word;
      std::string POS, semantic_role, pos2, UK1, UK2, UK3;
      out << word_id << "\t"
	  << raw_word << "\t"
	  << lc_word <<  "\t"
	  << UK1 << "\t"
	  << POS <<  "\t"
	  << semantic_role <<  "\t"
	  << parent_id <<  "\t"
	  << pos2 <<  "\t"
	  << UK2 <<  "\t"
	  << UK3 << std::endl;
    }
  out << std::endl;
};
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


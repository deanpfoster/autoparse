// -*- c++ -*-


#include "tokenize.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Tokenize::~Tokenize()
{
  m_input.close();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Tokenize::Tokenize(const std::string & name)
  :
  m_file_name(name),
  m_input(name)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
auto_parse::Words
auto_parse::Tokenize::next_sentence() 
{
  std::string one_line;
  getline(m_input,one_line);
  m_input >> std::ws; 
  std::stringstream s(one_line);
  Words result;
  while(!s.eof())
    {
      std::string next_word;
      s >> next_word;
      result.push_back(next_word);
    }
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Tokenize::reset() 
{
  m_input.close();
  m_input.open(m_file_name);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool
auto_parse::Tokenize::eof() const
{
  return m_input.eof();
};

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


// -*- c++ -*-


#include "eigenwords.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//
//                    STATIC
//
////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::map<std::string, Eigen::VectorXd>*> auto_parse::Eigenwords::s_cache(0);
std::vector<int> auto_parse::Eigenwords::s_cache_counter(0);



////////////////////////////////////////////////////////////////////////////////////////////
//
//  The following function is defined at the end of this file.
//  It was cribbed from wiki/code/google/CSV_grams.*
//
std::map<std::string,Eigen::VectorXd>
read_CSV(std::istream& in, int pos, int gram_number);

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Eigenwords::~Eigenwords()
{
  assert(m_alive);
  m_alive = false;
  assert(m_cache_index >= 0);
  assert(m_cache_index < static_cast<int>(s_cache_counter.size()));
  assert(s_cache_counter.size() == s_cache.size());

  assert(s_cache_counter.size() == 1); // this one should be removed when we have more than one eigendictionary
  s_cache_counter[m_cache_index]--;
  if(s_cache_counter[m_cache_index] == 0)
    {
      std::cout << "Should delete a cached eigendictionary." << std::endl;
      std::cout << "But we aren't going to since it seems to be having problems when we do so." << std::endl;
      //      delete(s_cache[m_cache_index]);
      //      s_cache[m_cache_index] = 0;  // we don't bother recovering the empty space left behind
    };
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords(std::istream& in, int gram_number)
  :  m_alive(true),
     m_cache_index(-1),
     mp_eigenwords()
{
  m_cache_index = s_cache.size();
  s_cache_counter.push_back(1);
  s_cache.push_back(new std::map<std::string,Eigen::VectorXd>);
  *s_cache[m_cache_index] = read_CSV(in, 0, gram_number);
  mp_eigenwords = s_cache[m_cache_index];
  assert(mp_eigenwords->find("<OOV>") != mp_eigenwords->end());
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords()
  :  m_alive(true),
     m_cache_index(-1),
     mp_eigenwords()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords(const auto_parse::Eigenwords& other)
  :  m_alive(true),
     m_cache_index(other.m_cache_index),
     mp_eigenwords(other.mp_eigenwords)
{
  s_cache_counter[m_cache_index]++;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
const Eigen::VectorXd&
auto_parse::Eigenwords::operator[](const auto_parse::Word& w) const
{
  auto location = mp_eigenwords->find(w);
  if(location != mp_eigenwords->end())
    return location->second;
  location = mp_eigenwords->find("<OOV>");
  if(location == mp_eigenwords->end())
    {
      std::cout << "Struggling with " << w << std::endl;
      assert(0);
    };
  return location->second;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Eigenwords::dimension() const
{
  return mp_eigenwords->begin()->second.size();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords
auto_parse::Eigenwords::with_constant_row_sum_squares() const
{
  // make a place on the cache for the result to live
  s_cache.push_back(new std::map<std::string, Eigen::VectorXd>);
  s_cache_counter.push_back(1);
  int index = s_cache_counter.size()-1;
  // make our return object actually point at the same place
  auto_parse::Eigenwords result;  // yes, we are calling the default constructor
  result.m_cache_index = index;
  result.mp_eigenwords = s_cache[index];
  for(auto i = mp_eigenwords->begin(); i != mp_eigenwords->end(); ++i)
    {
      Eigen::VectorXd values = i->second;
      double ss = values.norm();
      (*s_cache[index])[i->first] = i->second / ss;
    }
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions




#include <map>
#include <string>
#include <iostream>
#include <Eigen/QR>
#include <vector>
#include <set>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <iomanip>
#include "utilities/string_handling.h"


class Rest_of_row   // see parse_csv.cc for another example
{
public:
  Rest_of_row():
    m_ziff(0),
    m_freq(0.0),
    m_POS(""),
    m_index(0),
    m_counts()
  {};

  //  std::string m_word ;
  int  m_ziff;
  double m_freq;
  std::string m_POS;
  int m_index;
  Eigen::VectorXd m_counts;
};

std::istream&
operator>>(std::istream& in, std::pair<std::string, Rest_of_row>& full)
{
  typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer ;
  std::string line;
  getline(in,line);
  tokenizer toker(line);
  tokenizer::iterator iterator = toker.begin() ;
  full.first          = my_lex<std::string>(*iterator++) ;
  full.second.m_ziff  = my_lex<int>        (*iterator++);
  full.second.m_freq  = my_lex<double>     (*iterator++);
  full.second.m_POS   = my_lex<std::string>(*iterator++);
  full.second.m_index = my_lex<int>        (*iterator++);
  std::vector<double> tmp;
  while(iterator != toker.end())
    tmp.push_back(my_lex<double>(*iterator++));
  Eigen::VectorXd tmp2(tmp.size());
  for(unsigned int i = 0; i < tmp.size(); ++i)
    tmp2[i] = tmp[i];
  full.second.m_counts = tmp2;
  return in;
}


//////////////////////////////////////////////////////////////////////////////////////////
std::map<std::string,Eigen::VectorXd>
read_CSV(std::istream& in, int pos, int gram_number)
{
  std::vector<std::string> headers;
  assert(gram_number > pos);

  { // read in the headers--which we will toss away
    std::string line;
    getline(in, line);
    in >> std::ws;
    typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer ;
    tokenizer toker(line);
    for(tokenizer::iterator i = toker.begin(); i != toker.end(); ++i)
      headers.push_back(my_lex<std::string>(*i)) ;
  }
  std::map<std::string,Rest_of_row> tmp;
  { // read the data
    std::string line ;
    while( getline( in, line ) )
      tmp.insert(my_lex<std::pair<std::string,Rest_of_row> >(line) ); 
  }
  int data_size = tmp.begin()->second.m_counts.size();
  int state_size = data_size / gram_number;
  std::map<std::string,Eigen::VectorXd> result;
  for(auto i = tmp.begin(); i != tmp.end(); ++i)
    {
      result[i->first] = i->second.m_counts.segment(pos * state_size, (pos+1) * state_size); // segment(start, length)
    };
  return result;
}
//////////////////////////////////////////////////////////////////////////////////////////

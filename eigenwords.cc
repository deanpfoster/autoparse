// -*- c++ -*-


#include "eigenwords.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//
//                    STATIC
//
////////////////////////////////////////////////////////////////////////////////////////////

std::vector<auto_parse::Lexicon> auto_parse::Eigenwords::s_lexicon;
std::vector<auto_parse::Matrix*> auto_parse::Eigenwords::s_data;
std::vector<int> auto_parse::Eigenwords::s_which_lexicon;
std::vector<int> auto_parse::Eigenwords::s_cache_counter;


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
  assert(m_alive); // checks for double delete
  m_alive = false;
  assert(m_cache_index >= 0);
  assert(m_cache_index < static_cast<int>(s_cache_counter.size()));
  assert(s_cache_counter.size() == s_data.size());

  s_cache_counter[m_cache_index]--;
  bool brave = true;
  if(s_cache_counter[m_cache_index] == 0)
    {
      if(brave)
	{
	  // delete(s_cache[m_cache_index]); // no longer a pointer
          s_data[m_cache_index] = new Matrix(0,0); // this effectively deletes it
	}
      else
	{
	  std::cout << "Should delete cache dictionary " << m_cache_index << "." << std::endl;
	  std::cout << "But we aren't going to since it seems to be having problems when we do so." << std::endl;
	}
    };
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords(std::istream& in, int gram_number)
  :  m_alive(true),
     m_cache_index(-1),
     m_lexicon_index(-1),
     mp_data(),
     mp_lexicon()
{
  std::map<std::string,Eigen::VectorXd> from_disk = read_CSV(in, 0, gram_number);
  assert(from_disk.size() != 0); // assume we have read something real
  std::vector<std::string> raw_lex = generate_lexicon(from_disk);
  Lexicon lexicon(raw_lex);
  int d = from_disk.begin()->second.size();
  int n = raw_lex.size();
  Matrix data(n,d);
  for(unsigned int i = 0; i < raw_lex.size(); ++i)
    data.row(i) = from_disk[raw_lex[i]];

  m_cache_index = s_data.size();
  m_lexicon_index = s_lexicon.size();
  s_lexicon.push_back(lexicon);
  s_lexicon[m_lexicon_index].set_cache_id(m_lexicon_index);
  s_data.push_back(new Matrix(data));
  s_which_lexicon.push_back(m_lexicon_index);
  s_cache_counter.push_back(1);
  
  mp_data = s_data[m_cache_index];
  mp_lexicon = &s_lexicon[m_lexicon_index];
  assert(lexicon(lexicon.oov()) >= 0); // looks to ensure <OOV> is found
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords(std::istream& in)
  :  m_alive(true),
     m_cache_index(-1),
     m_lexicon_index(-1),
     mp_data(),
     mp_lexicon()
{
  std::string eigenwords;
  getline(in,eigenwords);
  assert(eigenwords == "auto_parse::Eigenwords");
  in >> std::ws;
  in >> m_lexicon_index;
  assert(m_lexicon_index <= int(s_lexicon.size()));  // must be writen in order
  if(m_lexicon_index < int(s_lexicon.size()))
    { // it has been read in before, so reading is not necessary
      std::string check;
      getline(in,check);
      in >> std::ws;
      assert(check == "Use a previously read lexicon.");
    }
  else
    { // this one is new
      assert(m_lexicon_index == int(s_lexicon.size()));
      Lexicon lexicon(in);
      lexicon.set_cache_id(m_lexicon_index);
      s_lexicon.push_back(lexicon);
    }

  in >> m_cache_index;
  assert(m_cache_index <= int(s_data.size()));  // must be writen in order
  if(m_cache_index < int(s_data.size()))
    { // it has been read in before, so reading is not necessary
      std::string check;
      getline(in,check);
      in >> std::ws;
      assert(check == "Use a previously read Eigenwords dictionary.");
    }
  else
    { // this one is new
      assert(m_cache_index == int(s_data.size()));
      int cache_index, dimension, n;
      in >> cache_index >> dimension >> n >> std::ws;
      assert(m_cache_index == cache_index);
      Matrix data(n,dimension);
      for(int i = 0; i < n; ++i)
	for(int j = 0; j < dimension; ++j)
	  in >> data(i,j) >> std::ws;
      s_data.push_back(new Matrix(data));
      s_which_lexicon.push_back(m_lexicon_index);
      s_cache_counter.push_back(1);
    }
  mp_data = s_data[m_cache_index];
  mp_lexicon = &s_lexicon[m_lexicon_index];
  assert((*mp_lexicon)(mp_lexicon->oov()) >= 0);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords()
  :  m_alive(true),
     m_cache_index(-1),
     m_lexicon_index(-1),
     mp_data(),
     mp_lexicon()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords(const auto_parse::Eigenwords& other)
  :  m_alive(true),
     m_cache_index(other.m_cache_index),
     m_lexicon_index(other.m_lexicon_index),
     mp_data(other.mp_data),
     mp_lexicon(other.mp_lexicon)
{
  s_cache_counter[m_cache_index]++;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords::Eigenwords(int i1, int i2, int i3)
  :  m_alive(true),
     m_cache_index(i1),
     m_lexicon_index(s_which_lexicon[i2]),
     mp_data(s_data[m_cache_index]),
     mp_lexicon(&s_lexicon[m_lexicon_index])
{
  assert(i1 == i2);  // this is the secrete password to call this constructor
  assert(i2 == i3);
  s_cache_counter[i3]++;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords
auto_parse::Eigenwords::create_root_dictionary(const auto_parse::Lexicon& lexicon)
{
  if(lexicon.cache_id() == -1)
    {  // cache it
      int lexicon_index = s_lexicon.size();
      s_lexicon.push_back(lexicon);
      s_which_lexicon.push_back(lexicon_index);
      s_lexicon[lexicon_index].set_cache_id(lexicon_index);
    };
  static int root_dictionary_id = -1;  // this will be changed
  if(root_dictionary_id == -1)
    { // no root_dictionary created yet
      root_dictionary_id = s_data.size();
      s_cache_counter.push_back(0); // we aren't keeping a copy
      s_data.push_back(new Matrix(lexicon.size(),1));
      for(int i = 0; i < lexicon.size(); ++i)
	(*s_data[root_dictionary_id])(i,0) = 1.0; // set it up to be a constand matrix
      s_which_lexicon.push_back(lexicon.cache_id());
    }
  return Eigenwords(root_dictionary_id, root_dictionary_id, root_dictionary_id);  // use secret password constructor
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
Eigen::VectorXd
auto_parse::Eigenwords::operator[](const auto_parse::Word& w) const
{
  int location = w.as_index();
  if(location >= mp_lexicon->size())
    {
      std::cout << "Struggling with " << location << " :" << w.convert_to_string(lexicon()) << std::endl;
      assert(0);
    };
  assert(location >= 0);
  assert(location < mp_data->rows());
  Eigen::VectorXd result = mp_data->row(location);
  return result;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Eigen::VectorXd
auto_parse::Eigenwords::operator()(const auto_parse::Node& pointer, const auto_parse::Words& sentence) const
{
  if(pointer == sentence.end())
    return((*this)[Word(lexicon(),"")]);
  else
    return((*this)(*pointer));
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int
auto_parse::Eigenwords::dimension() const
{
  return mp_data->cols();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Eigenwords
auto_parse::Eigenwords::with_constant_row_sum_squares() const
{
  // make a place on the cache for the result to live
  s_data.push_back(new Matrix(*mp_data)); // copy current data into new location
  s_cache_counter.push_back(1);
  s_which_lexicon.push_back(m_lexicon_index);
  int index = s_cache_counter.size()-1;
  for(int i = 0; i < mp_data->rows(); ++i)
    (*s_data[index]).row(i) = mp_data->row(i) / mp_data->row(i).norm();
  return   auto_parse::Eigenwords(index,index,index);  
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected
////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private
std::vector<std::string>
auto_parse::Eigenwords::generate_lexicon(const std::map<std::string,Eigen::VectorXd>& from_disk) const
{
  std::vector<std::string> result;
  for(auto a : from_disk)
    result.push_back(a.first);
  return result;
}
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

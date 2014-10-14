// -*- c++ -*-

#ifndef INCLUDED_EIGENWORDS
#define INCLUDED_EIGENWORDS

#include "word.h"
#include <map>
#include <Eigen/Core>
#include <fstream>
#include "vector.h"

namespace auto_parse
{
  class Eigenwords
  {
  public:
    // CONSTRUCTORS
    ~Eigenwords();
    Eigenwords(std::istream& in, int gram_number); // reads a pretty.csv
    Eigenwords(std::istream&);  // reads what an eigenword save generated
    Eigenwords(const Eigenwords&);
    static Eigenwords create_root_dictionary(const Lexicon&);
    static void debug_dictionary(const Lexicon&);

    // MANIPULATORS
    // ACCESSORS
    Vector operator()(const Word&w) const{return (*this)[w];}; // maybe return Matrix::ConstRowXpr
    Vector operator[](const Word&) const;
    Vector operator()(const Node&, const Words&) const;
    int dimension() const;
    int size() const{return lexicon().size();};
    Eigenwords with_constant_row_sum_squares() const;
    const Lexicon& lexicon() const{return *mp_lexicon;};
    const Lexicon* p_lexicon() const{return mp_lexicon;};
  private:
    // DATA
    bool m_alive;
    int m_cache_index;
    int m_lexicon_index;
    Matrix* mp_data;
    Lexicon* mp_lexicon;

    // DELETED FUNCTIONS
    void operator=(const Eigenwords&);     // can't be called
    Eigenwords();                          // can't be called

    // HELPER FUNCTIONS
    std::vector<std::string> generate_lexicon(const std::map<std::string,Vector>&) const;
    Eigenwords(int,int,int);  // create from cache ID

    // STATIC DATA
    static std::vector<Lexicon> s_lexicon;
    static std::vector<Matrix*> s_data;  // can't store matrixes themselves since alignment gets off when vector is moved.
    static std::vector<int> s_which_lexicon;
    static std::vector<int> s_cache_counter;
  };
}

#endif

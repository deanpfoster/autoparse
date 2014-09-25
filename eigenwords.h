// -*- c++ -*-

#ifndef INCLUDED_EIGENWORDS
#define INCLUDED_EIGENWORDS

#include "word.h"
#include <map>
#include <Eigen/Core>
#include <fstream>

namespace auto_parse
{
  // Note:  typedef Matrix<double, Dynamic, Dynamic, RowMajor> Eigen::MatrixXd;
  // So using a MatrixXd is column major order which is
  // WRONG for accessing a row at a time in a fast fashion.
  // If we need to call serious Eigen code (i.e. SVD / LU) then it might be
  // worth while changing rows and columns around since Eigen code is setup
  // to use MatrixXd by default and so might run faster / better.
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Matrix;
  //  typedef Eigen::MatrixXd Matrix;

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
    Eigen::VectorXd operator()(const Word&w) const{return (*this)[w];};
    Eigen::VectorXd operator[](const Word&) const;
    Eigen::VectorXd operator()(const Node&, const Words&) const;
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
    Eigen::MatrixXd* mp_data;
    Lexicon* mp_lexicon;

    // DELETED FUNCTIONS
    void operator=(const Eigenwords&);     // can't be called
    Eigenwords();                          // can't be called

    // HELPER FUNCTIONS
    std::vector<std::string> generate_lexicon(const std::map<std::string,Eigen::VectorXd>&) const;
    Eigenwords(int,int,int);  // create from cache ID

    // STATIC DATA
    static std::vector<Lexicon> s_lexicon;
    static std::vector<Eigen::MatrixXd*> s_data;  // can't store matrixes themselves since alignment gets off when vector is moved.
    static std::vector<int> s_which_lexicon;
    static std::vector<int> s_cache_counter;
  };
}

#endif

// -*- c++ -*-

#ifndef INCLUDED_EIGENWORDS
#define INCLUDED_EIGENWORDS

#include "word.h"
#include <map>
#include <Eigen/Core>
#include <fstream>

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
    static Eigenwords create_root_dictionary();

    // MANIPULATORS
    // ACCESSORS
    const Eigen::VectorXd& operator()(const Word&w) const{return (*this)[w];};
    const Eigen::VectorXd& operator[](const Word&) const;
    const Eigen::VectorXd& operator()(const Node&, const Words&) const;
    std::map<std::string,Eigen::VectorXd>::const_iterator find(const Word& w) const;
    std::map<std::string,Eigen::VectorXd>::const_iterator end() const{return mp_eigenwords->end();}
    int dimension() const;
    int size() const{return mp_eigenwords->size();};
    Eigenwords with_constant_row_sum_squares() const;
  private:
    bool m_alive;
    int m_cache_index;
    std::map<std::string,Eigen::VectorXd>* mp_eigenwords;
    void operator=(const Eigenwords&);     // can't be called
    Eigenwords();                          // can't be called

    // STATIC

    static std::vector<std::map<std::string,Eigen::VectorXd>* > s_cache;
    static std::vector<int> s_cache_counter;
    Eigenwords(int,int,int);  // create from cache ID
  };
}

#endif

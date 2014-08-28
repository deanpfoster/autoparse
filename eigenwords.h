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
    Eigenwords(std::istream& in, int gram_number);
    Eigenwords(const Eigenwords&);

    // MANIPULATORS
    // ACCESSORS
    const Eigen::VectorXd& operator()(const Word&w) const{return (*this)[w];};
    const Eigen::VectorXd& operator[](const Word&) const;
    int dimension() const;
    int size() const{return mp_eigenwords->size();};
  private:
    bool m_alive;
    int m_cache_index;
    std::map<std::string,Eigen::VectorXd>* mp_eigenwords;
    void operator=(const Eigenwords&);     // can't be called
    Eigenwords();                          // can't be called

    // STATIC

    static std::vector<std::map<std::string,Eigen::VectorXd>* > s_cache;
    static std::vector<int> s_cache_counter;


  };
}

#endif

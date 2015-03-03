// -*- c++ -*-

#ifndef INCLUDED_FEATURE_GENERATOR
#define INCLUDED_FEATURE_GENERATOR

#include <iosfwd>
#include <vector>
#include "feature.h"
#include "word.h"
#include <initializer_list>
#include "history.h"
#include <Eigen/Core>

namespace auto_parse
{
  class LR;
  class Feature_generator
  {
  public:
    // CONSTRUCTORS
    ~Feature_generator();
    Feature_generator();
    Feature_generator(const Feature_generator &);    
    Feature_generator(std::istream &);    
    Feature_generator& add(const std::initializer_list<Feature*>&);
    Feature_generator& add(const Feature&);
    Feature_generator& add(const std::vector<Feature*>&);
    Feature_generator& add(const Feature_generator&);

    // MANIPULATORS
    // ACCESSORS
    Eigen::VectorXd operator()(const LR& lr) const; // use features() for auto_parse::Vector.
    void print_on(std::ostream &) const;
    
    Vector features(const LR&) const;
    std::vector<std::string> feature_names() const;     // one word per feature: "TOS_1", "TOS_2", "TOS_3", ..., "size_of_stack", etc
    std::vector<std::string> feature_summaries() const; // one word per class: "Eigenword(top of stack)", "size of stack", etc
    int dimension() const{return m_number_features;};

  protected:
  private:
    int m_number_features;
    std::vector<Feature*> m_features;  // can't store features since ABC

    Feature_generator& operator=(const Feature_generator &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Feature_generator &);
std::ostream& operator<<(std::ostream &,const std::vector<std::string> &);

#endif

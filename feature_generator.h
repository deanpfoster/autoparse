// -*- c++ -*-

#ifndef INCLUDED_FEATURE_GENERATOR
#define INCLUDED_FEATURE_GENERATOR

#include <iosfwd>
#include <vector>
#include "feature.h"
#include "word.h"
#include <initializer_list>
#include "history.h"

namespace auto_parse
{
  class LR;
  class Feature_generator
  {
  public:
    // CONSTRUCTORS
    ~Feature_generator();
    Feature_generator(); // Wants to have an Eigen_Dictionary passed in
    Feature_generator(const std::vector<Feature*>&);
    Feature_generator(const std::initializer_list<Feature*>&);
    

    // MANIPULATORS
    // ACCESSORS
    void print_on(std::ostream &) const;
    void write_row(std::ostream&,
		   const Words&,
		   const History&,
		   Action, double,
		   Action, double) const;
    std::vector<double> features(const LR&) const;
    std::vector<std::string> feature_names() const;     // one word per feature: "TOS_1", "TOS_2", "TOS_3", ..., "size_of_stack", etc
    std::vector<std::string> feature_summaries() const; // one word per class: "Eigenword(top of stack)", "size of stack", etc

  protected:
  private:
    int m_number_features;
    std::vector<Feature*> m_features;  // can't store features since ABC

    Feature_generator(const Feature_generator &);            // Don't delete this.
    Feature_generator& operator=(const Feature_generator &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Feature_generator &);

#endif

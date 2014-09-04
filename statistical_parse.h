// -*- c++ -*-

#ifndef INCLUDED_STATISTICAL_PARSE
#define INCLUDED_STATISTICAL_PARSE

#include <iosfwd>
#include "model.h"
#include "lr.h"
#include "feature_generator.h"

namespace auto_parse
{
  class Statistical_parse
  {
  public:
    // CONSTRUCTORS
    ~Statistical_parse();
    Statistical_parse(const Model&, const Feature_generator&,double default_noise_level);
    Statistical_parse(const Statistical_parse &); 

    // MANIPULATORS
    void new_model(const Model& new_model){m_model = new_model;};
    // ACCESSORS
    Model model() const{return m_model;};
    History best_parse(const Words&) const; 
    History operator()(const Words&) const; 
    History best_parse_finish(const Words&, const History& prefix) const; 
    History finish(const Words&, const History& prefix) const; 

  protected:
  private:
    History private_finish(const Words&, const History& prefix,double noise) const; 
    History do_actual_parse(LR*, double) const;
    Model m_model;
    Feature_generator m_generator;
    double m_noise;
  };
}

#endif

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

    History operator()(const Words&, double noise_level = -1) const; 
    History finish(const Words&, const History& prefix, double noise_leve = -1) const; 

  protected:
  private:
    History do_actual_parse(LR*, double) const;
    Model m_model;
    Feature_generator m_generator;
    double m_noise;
  };
}

#endif

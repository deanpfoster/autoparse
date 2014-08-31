// -*- c++ -*-

#ifndef INCLUDED_LEARN
#define INCLUDED_LEARN

#include <iostream>
#include <assert.h>
#include <fstream>

#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"
#include "redo_parse.h"
#include "feature_generator.h"
#include "feature_one_dimensional.h"
#include "forecast_linear.h"
#include "eigenwords.h"
#include "tp_eigenwords.h"
#include "maximum_likelihood.h"
#include "feature_eigenwords.h"
#include "train_forecast_linear.h"
#include "row.h"
#include "contrast.h"
#include "tokenize.h"


namespace auto_parse
{
  int number_of_threads_used();
  
    Feature_generator
      fast_features(const Eigenwords& dictionary);

    Feature_generator
      standard_features(const Eigenwords& dictionary);

  
  Model
    standard_model(const Feature_generator& feature_generator);

  Model
    likelihood_to_model(const Likelihood& likelihood,
			const auto_parse::Statistical_parse& parser,
			const Feature_generator& feature_generator,
			const Model& lr_model,
			double sampling_rate,
			std::vector<auto_parse::Words>::const_iterator,
			std::vector<auto_parse::Words>::const_iterator);

  Likelihood
    model_to_likelihood(const Eigenwords& parent,const Eigenwords& child,
			const auto_parse::Statistical_parse& parser,
			std::vector<auto_parse::Words>::const_iterator,
			std::vector<auto_parse::Words>::const_iterator);
  std::string
  evaluation(int rounds,
	     std::ostream& debugging,
	     std::ostream& latex,
	     const Eigenwords& dictionary,
	     const Statistical_parse& parser,
	     const Likelihood& likelihood,
	     std::vector<int>& which_sentences,
	     std::vector<auto_parse::Words>::const_iterator begin,
	     std::vector<auto_parse::Words>::const_iterator end);




}

#endif

// -*- c++ -*-

#ifndef INCLUDED_SAMPLE
#define INCLUDED_SAMPLE

#include <iostream>
#include <assert.h>
#include <fstream>

#include "sample.h"

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
			const std::vector<auto_parse::Words>& corpus_in_memory,
			std::ostream& debugging, std::string debugging_prefix);

  Likelihood
    model_to_likelihood(const Eigenwords& dictionary,
			const std::vector<auto_parse::Words>& corpus_in_memory,
			const auto_parse::Statistical_parse& parser);


}

#endif

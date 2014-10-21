// -*- c++ -*-

#ifndef INCLUDED_FORK
#define INCLUDED_FORK

#include "feature_generator.h"
#include "eigenwords.h"
#include "model.h"
#include "statistical_parse.h"
#include "gold_standard.h"





  Model golden_fork(const auto_parse::Statistical_parse& parser,
		    const Feature_generator& feature_generator,
		    double sampling_rate,
		    std::vector<Gold_standard>::const_iterator gold_begin,
		    std::vector<auto_parse::Words>::const_iterator begin,
		    std::vector<auto_parse::Words>::const_iterator end);

};

#endif

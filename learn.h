// -*- c++ -*-

#ifndef INCLUDED_LEARN
#define INCLUDED_LEARN
#include "feature_generator.h"
#include <boost/tuple/tuple.hpp>
#include "eigenwords.h"
#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"


namespace auto_parse
{
  int number_of_threads_used();

  Feature_generator fast_features();
  Feature_generator standard_features(const Eigenwords& dictionary);
  Feature_generator eager_features(const Eigenwords& dictionary);
  
  Model generate_linear_model(int);
  Model likelihood_to_model(const Likelihood& likelihood,
			    const auto_parse::Statistical_parse& parser,
			    const Feature_generator& feature_generator,
			    double sampling_rate,
			    std::vector<auto_parse::Words>::const_iterator,
			    std::vector<auto_parse::Words>::const_iterator,
			    std::ostream&);

  Likelihood
    model_to_likelihood(const Eigenwords& parent,const Eigenwords& child,
			const auto_parse::Statistical_parse& parser,
			double scaling,
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


  boost::tuple<int, std::string, std::string, int, std::string, double, double, double, std::string,bool>
  parse_argv(int argc, char** argv);


}

#endif

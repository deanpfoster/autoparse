// -*- c++ -*-

#ifndef INCLUDED_LEARN
#define INCLUDED_LEARN
#include "feature_generator.h"
#include <boost/tuple/tuple.hpp>
#include "eigenwords.h"
#include "model.h"
#include "likelihood.h"
#include "statistical_parse.h"
#include "gold_standard.h"
#include "parse_args.h"


namespace auto_parse
{
  int number_of_threads_used();

  Feature_generator fast_features();
  Feature_generator standard_features(const Eigenwords& dictionary);
  Feature_generator eager_features(const Eigenwords& dictionary);
  
  Model generate_linear_model(int, const std::vector<auto_parse::Action>&);
  Model likelihood_to_model(const Likelihood& likelihood,
			    const auto_parse::Statistical_parse& parser,
			    const Feature_generator& feature_generator,
			    double sampling_rate,
			    std::vector<auto_parse::Words>::const_iterator,
			    std::vector<auto_parse::Words>::const_iterator,
			    std::ostream&);

  Model gold_standard_to_model(const auto_parse::Statistical_parse& parser,
			       const Feature_generator& feature_generator,
			       double sampling_rate,
			       std::vector<Gold_standard>::const_iterator gold_begin,
			       std::vector<auto_parse::Words>::const_iterator begin,
			       std::vector<auto_parse::Words>::const_iterator end);


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

  std::string
  golden_evaluation(const Statistical_parse& parser,
		    std::vector<auto_parse::Words>::const_iterator begin,
		    std::vector<auto_parse::Words>::const_iterator end,
		    std::vector<auto_parse::Gold_standard>::const_iterator gold_begin);

  
  typedef boost::tuple<std::string, std::string, int, std::string> File_names;

  std::string friendly_message(const Parse_args&,
			       const auto_parse::Eigenwords&,
			       const std::vector<auto_parse::Words>&) ; 

  void print_latex(const Parse_args&,
		   const std::vector<auto_parse::Words>& corpus,
		   const auto_parse::Likelihood& likelihood,
		   const std::vector<int>& number_to_train_on,
		   const auto_parse::Eigenwords& dictionary,
		   const auto_parse::Statistical_parse& parser) ;



  std::string print_time(const std::string& h);


};

#endif

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

  
  typedef boost::tuple<std::string, std::string, int, std::string> File_names;

  class Parse_args
  {
  public:
    Parse_args(int argc, char** argv);  // constructs and packs all the variables into a bundle

    std::string friendly_message(const auto_parse::Eigenwords&,
				 const std::vector<auto_parse::Words>&) const; 

    void print_latex(time_t start_time,
		     const std::vector<auto_parse::Words>& corpus,
		     const auto_parse::Likelihood& likelihood,
		     const std::vector<int>& number_to_train_on,
		     const auto_parse::Eigenwords& dictionary,
		     const auto_parse::Statistical_parse& parser) const;

    // DATA  (exposed to the world)
    std::string sentence_file, eigen_file, latex_prefix;
    int gram_number, repeats_per_level;
    double update_rate,scaling, noise;
    bool use_eager, r2l;
    std::string comment;
  };

  std::string print_time(const std::string& h);


};

#endif

// -*- c++ -*-


#include "learn.h"
#include "assert.h"
#include <iostream>
#include "feature_interaction.h"
#include <boost/program_options.hpp>

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::vector<auto_parse::Feature*>
linear_features(const auto_parse::Eigenwords& dictionary)
{
  using namespace auto_parse;
  // the following uses new C++11 standard to shove all these into a vector
  typedef Feature_one_dimensional<Words_left> F_wl;
  typedef Feature_one_dimensional<Stack_size> F_ss;
  typedef Feature_one_dimensional<Sentence_length> F_sl;
  typedef Feature_one_dimensional<Distance_to<0> > F_d0;
  typedef Feature_one_dimensional<Distance_to<1> > F_d1;
  typedef Feature_one_dimensional<Distance_to<2> > F_d2;
  std::vector<Feature*> result
  {
    new F_wl,
      new Interaction<F_wl, F_wl>(F_wl(),F_wl()),
      new F_ss,
      new Interaction<F_ss, F_ss>(F_ss(),F_ss()),
      new F_sl,
      new Interaction<F_sl, F_sl>(F_sl(),F_sl()),
      new Interaction<F_sl, F_wl>(F_sl(),F_wl()),
      new Interaction<F_sl, F_ss>(F_sl(),F_ss()),
      new Interaction<F_ss, F_wl>(F_ss(),F_wl()),
      new F_d0,
      new F_d1,
      new F_d2,
      new Feature_eigenwords<Next_word>(dictionary),
      new Feature_eigenwords<Stack_top>(dictionary),
      new Feature_eigenwords<Stack_1>(dictionary)
      };
  return result;
}

std::vector<auto_parse::Feature*>
eigen_interactions(const auto_parse::Eigenwords& dictionary)
{
  using namespace auto_parse;
  // the following uses new C++11 standard to shove all these into a vector
  typedef Feature_eigenwords<Next_word> nw;
  typedef Feature_eigenwords<Stack_top> st;
  typedef Feature_eigenwords<Stack_1>   s1;
  
  std::vector<Feature*> result
  {
    new Interaction<st, s1>(st(dictionary),s1(dictionary))
  };
  return result;
}

auto_parse::Feature_generator
auto_parse::fast_features(const Eigenwords& dictionary)
{
  std::vector<Feature*> features = linear_features(dictionary);
  Feature_generator result = Feature_generator(features);
  return result;
};

auto_parse::Feature_generator
auto_parse::standard_features(const Eigenwords& dictionary)
{
  std::vector<Feature*> features = linear_features(dictionary);
  std::vector<Feature*> more = eigen_interactions(dictionary);
  std::copy(more.begin(), more.end(), std::back_inserter(features));
  Feature_generator result = Feature_generator(features);
  return result;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Model
auto_parse::standard_model(const Feature_generator& feature_generator)
{
  int num_features = feature_generator.dimension();
  Forecast* p_example_shift = new Forecast_linear(Eigen::VectorXd::Zero(num_features));
  Forecast* p_example_left  = new Forecast_linear(Eigen::VectorXd::Zero(num_features));
  Forecast* p_example_right = new Forecast_linear(Eigen::VectorXd::Zero(num_features));
  Forecast* p_example_reduce= new Forecast_linear(Eigen::VectorXd::Zero(num_features));
  Model result(
	       {   {Action::shift,       p_example_shift},
		   {Action::left_reduce, p_example_left},
		   {Action::right_reduce,p_example_right},
		   {Action::head_reduce, p_example_reduce}
	       },
	       feature_generator);
  return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int
auto_parse::number_of_threads_used()
{
  int result;
#pragma omp parallel default(shared)
  result = omp_get_num_threads();
  return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Model
auto_parse::likelihood_to_model(const Likelihood& likelihood,
				const auto_parse::Statistical_parse& parser,
				const Feature_generator& feature_generator,
				double sampling_rate,
				std::vector<auto_parse::Words>::const_iterator begin,
				std::vector<auto_parse::Words>::const_iterator end)
{
  std::map<auto_parse::Action, auto_parse::Train_forecast_linear> training;
  Model lr_model = parser.model();
  for(auto_parse::Action a: auto_parse::all_actions)
    training[a] = auto_parse::Train_forecast_linear(lr_model.forecast(a),sampling_rate);

  std::vector<std::map<auto_parse::Action, auto_parse::Train_forecast_linear> > training_bundle(0);
  int num_threads;
  auto_parse::Contrast contrast(parser, likelihood, feature_generator);

#pragma omp parallel default(shared)
  {
#pragma omp single
    {
      num_threads = omp_get_num_threads();
      training_bundle = std::vector<std::map<auto_parse::Action, auto_parse::Train_forecast_linear> >(num_threads);
      for(int i = 0; i < num_threads; ++i)
	training_bundle[i] = training;
    }
    int number_to_read = end - begin;
#pragma omp for 
    for(int counter = 0; counter < number_to_read;++counter)
      {
	const auto_parse::Words& sentence = *(begin + counter);
	int thread_ID = omp_get_thread_num();
	std::vector<auto_parse::Row> contrast_pair = contrast(sentence);
	for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	  training_bundle[thread_ID][i->m_a](i->m_X, i->m_Y);
      };
  }
  for(int i = 1; i < num_threads;++i)
    for(auto_parse::Action a: auto_parse::all_actions)
      training_bundle[0][a].merge(training_bundle[i][a]);
  auto_parse::Model new_model = parser.model();
  for(auto_parse::Action a : auto_parse::all_actions)
    new_model.add_forecast(a,training_bundle[0][a].result());
  return new_model;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood
auto_parse::model_to_likelihood(const Eigenwords& parent,const Eigenwords& child,
				const auto_parse::Statistical_parse& parser,
				std::vector<auto_parse::Words>::const_iterator begin,
				std::vector<auto_parse::Words>::const_iterator end)
{
  auto_parse::TP_eigenwords left(parent,child); 
  auto_parse::TP_eigenwords right(parent,child); 
  int num_threads;
  std::vector<auto_parse::Maximum_likelihood> mle_bundle(0);
#pragma omp parallel default(shared)
  {
#pragma omp single
    {
      num_threads = omp_get_num_threads();
      mle_bundle = std::vector<auto_parse::Maximum_likelihood>(num_threads);
      //#pragma omp for 
    for(unsigned int i = 0; i < mle_bundle.size(); ++i)
	mle_bundle[i] = auto_parse::Maximum_likelihood(left, right);
    }
    int number_to_read = end - begin;
#pragma omp for 
    for(int counter = 0; counter < number_to_read;++counter)
      {	
	const auto_parse::Words& sentence = *(begin + counter);
	int thread_ID = omp_get_thread_num();
	assert(thread_ID < num_threads);
	auto_parse::Dependency d = redo_parse(sentence,parser(sentence)).parse();
	mle_bundle[thread_ID](d);  // adds to database
      }
  }
  for(int i = 1; i < num_threads;++i)
    mle_bundle[0].merge(mle_bundle[i]);

  Likelihood likelihood = mle_bundle[0].output();
  return likelihood;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
std::string
auto_parse::evaluation(int rounds,
		       std::ostream& debugging,
		       std::ostream& latex,
		       const Eigenwords& dictionary,
		       const Statistical_parse& parser,
		       const Likelihood& likelihood,
		       std::vector<int>& which_sentences,
		       std::vector<auto_parse::Words>::const_iterator begin,
		       std::vector<auto_parse::Words>::const_iterator end)
{
  double log_like = 0;
  int number_to_read = end - begin;
#pragma omp parallel for 
  for(int counter = 0; counter < number_to_read;++counter)
    {
      const auto_parse::Words& sentence = *(begin + counter);
      auto_parse::Dependency parse = redo_parse(sentence, parser(sentence)).parse();
      double prob = likelihood(parse) / (sentence.end() - sentence.begin());
      log_like += prob;
    };
  std::stringstream summary;
  int n = end - begin;
  summary << "log(like) = " << log_like/n << " on " << n/1000 << "k";

  if(which_sentences.size() > 0)
    {
      std::vector<auto_parse::Dependency> parses;
      for(int sentence_id : which_sentences)
	parses.push_back(redo_parse(*(begin + sentence_id), parser(*(begin + sentence_id))).parse());
      latex << "\\newpage\n\\section*{\\bf{" << rounds << ":}  " << summary.str() << "}\n\n" << std::endl;
      for(auto_parse::Dependency p : parses)
	likelihood.decorate(p, dictionary).latex(latex);
      debugging << likelihood.decorate(parses[0],dictionary);
    };
  return summary.str();
}
	
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

boost::tuple<int, std::string,std::string,int,std::string,double>
auto_parse::parse_argv(int argc, char** argv)
{
  namespace po = boost::program_options;

  std::string latex;
  std::string dictionary;
  std::string corpus;
  int         gram,repeats_per_level;
  double      update;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message (see parse_argv in learn.cc)")
    ("corpus", po::value<std::string>(&corpus)->default_value("eng_only"), "corpus to read from")
    ("latex", po::value<std::string>(&latex)->default_value("learn.output.tex"), "latex file to write to")
    ("dictionary", po::value<std::string>(&dictionary)->default_value("pretty.csv"), "dictionary to read from")
    ("gram_number", po::value<int>(&gram)->default_value(3), "gram number for dictionary")
    ("repeats_per_level", po::value<int>(&repeats_per_level)->default_value(3), "number of times to process at each size")
    ("update_rate", po::value<double>(&update)->default_value(1.0), "rate we move towards new data")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << "\n";
    exit(1);
  }

  return boost::make_tuple(repeats_per_level,latex, dictionary, gram, corpus,update);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

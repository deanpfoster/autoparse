// -*- c++ -*-


#include "sample.h"
#include "assert.h"
#include <iostream>
#include "feature_interaction.h"

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
  Forecast* p_example_shift = new Forecast_linear(Eigen::VectorXd(num_features));
  Forecast* p_example_left  = new Forecast_linear(Eigen::VectorXd(num_features));
  Forecast* p_example_right = new Forecast_linear(Eigen::VectorXd(num_features));
  Forecast* p_example_reduce= new Forecast_linear(Eigen::VectorXd(num_features));
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

auto_parse::Model
auto_parse::likelihood_to_model(const Likelihood& likelihood,
				const auto_parse::Statistical_parse& parser,
				const Feature_generator& feature_generator,
				const Model& lr_model,
				const std::vector<auto_parse::Words>& corpus_in_memory,
				std::ostream& debugging, std::string debugging_prefix)
{
  std::map<auto_parse::Action, auto_parse::Train_forecast_linear> training;
  for(auto_parse::Action a: auto_parse::all_actions)
    training[a] = auto_parse::Train_forecast_linear(lr_model.forecast(a));

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
#pragma omp for 
    for(unsigned int i = 0; i < corpus_in_memory.size(); ++i)
      {
	int thread_ID = omp_get_thread_num();
	auto sentence = corpus_in_memory[i];
	std::vector<auto_parse::Row> contrast_pair = contrast(sentence);
	for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	  training_bundle[thread_ID][i->m_a](i->m_X, i->m_Y);
      };
  }
  for(int i = 1; i < num_threads;++i)
    for(auto_parse::Action a: auto_parse::all_actions)
      training_bundle[0][a].merge(training_bundle[i][a]);
  auto_parse::Model new_model(feature_generator);
  for(auto_parse::Action a : auto_parse::all_actions)
    new_model.add_forecast(a,training_bundle[0][a].result());

  debugging << debugging_prefix  << "Trained on " << corpus_in_memory.size()
	    << " sentence.    " << "number threads = " << num_threads << ".";
  return new_model;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood
auto_parse::model_to_likelihood(const Eigenwords& dictionary,
				const std::vector<auto_parse::Words>& corpus_in_memory,
				const auto_parse::Statistical_parse& parser)
{
  int dim = dictionary.dimension();
  Eigen::MatrixXd identity = Eigen::MatrixXd::Identity(dim,dim); 
  auto_parse::TP_eigenwords left(dictionary,identity); 
  auto_parse::TP_eigenwords right(dictionary,identity); 
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
#pragma omp for 
    for(unsigned int i = 0; i < corpus_in_memory.size(); ++i)
      {	
	int thread_ID = omp_get_thread_num();
	assert(thread_ID < num_threads);
	auto sentence = corpus_in_memory[i];
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

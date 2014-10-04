// -*- c++ -*-


#include "learn.h"
#include "assert.h"
#include "feature_shorten.h"
#include <iostream>
#include "feature_interaction.h"
#include <boost/program_options.hpp>
#include "feature_interaction.h"
#include "feature_one_dimensional.h"
#include "feature_eigenwords.h"
#include "train_forecast_linear.h"
#include "row.h"
#include "contrast.h"
#include "tp_eigenwords.h"
#include "tp_iid.h"
#include "maximum_likelihood.h"
#include "redo_parse.h"

#include "forecast_linear.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

static int POS_length = 10; // Enough of the top eigen directions to compute a POS. (guessed value.)

auto_parse::Feature_generator
auto_parse::fast_features()
{  // these are one-D features that can be computed quickly.  
  Feature_generator result;
  Feature_one_dimensional<Sentence_length> sl;
  Feature_one_dimensional<Words_left>      wl;
  Feature_one_dimensional<Distance_to<0> > d0;
  Feature_one_dimensional<Distance_to<1> > d1;
  Feature_one_dimensional<Distance_to<2> > d2;
  Feature_one_dimensional<Stack_size>      ss;
  return result
    .add(sl)
    .add(wl)
    .add(d0)
    .add(d1)
    .add(d2)
    .add(ss)
    .add(sl * sl)
    .add(sl * wl)
    .add(sl * d0)
    .add(sl * d1)
    .add(sl * d2)
    .add(sl * ss)
    .add(wl * wl)
    .add(wl * d0)
    .add(wl * ss)
    .add(ss * ss)
    .add(ss *ss * ss)
    ;
};

auto_parse::Feature_generator
short_interaction_pairs(const auto_parse::Feature& a,
			const auto_parse::Feature& b,
			const auto_parse::Feature& c,
			int length)
{
  using namespace auto_parse;
  Feature_generator result;
  return result
    .add(shorten(a,length) * shorten(a, length))
    .add(shorten(a,length) * shorten(b, length))
    .add(shorten(a,length) * shorten(c, length))
    .add(shorten(b,length) * shorten(b, length))
    .add(shorten(b,length) * shorten(c, length))
    .add(shorten(c,length) * shorten(c, length));
}

auto_parse::Feature_generator
auto_parse::standard_features(const Eigenwords& dictionary)
{
  return Feature_generator()
    .add(fast_features())
    .add(short_interaction_pairs(Feature_eigenwords<Next_word>(dictionary),
				 Feature_eigenwords<Stack_top>(dictionary),
				 Feature_eigenwords<Stack_1>  (dictionary),
				 POS_length))
    .add(Feature_eigenwords<Next_word>(dictionary))
    .add(Feature_eigenwords<Stack_top>(dictionary))
    .add(Feature_eigenwords<Stack_1>(dictionary));
};

auto_parse::Feature_generator
auto_parse::eager_features(const Eigenwords& dictionary)
{
  return Feature_generator()
    .add(fast_features())
    .add(short_interaction_pairs(Feature_eigenwords<Next_word>(dictionary),
				 Feature_eigenwords<Stack_top>(dictionary),
				 Feature_eigenwords<Next_word_1>(dictionary),
				 POS_length))
    .add(shorten(Feature_eigenwords<Stack_top_left_most>(dictionary),POS_length))
    .add(shorten(Feature_eigenwords<Stack_top_right_most>(dictionary),POS_length))
    .add(shorten(Feature_eigenwords<Next_word_left_most>(dictionary),POS_length)) // can't have any right children...
    .add(Feature_eigenwords<Next_word>(dictionary))
    .add(Feature_eigenwords<Next_word_1>(dictionary))
    .add(Feature_eigenwords<Stack_top>(dictionary))
    ;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Model
auto_parse::generate_linear_model(int num_features)
{
  Model result;
  for(auto_parse::Action a: all_actions)
    result.add_forecast(a, Forecast_linear(Eigen::VectorXd::Zero(num_features)));
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
				std::vector<auto_parse::Words>::const_iterator end,
				std::ostream& ostrm)
{
  Model lr_model = parser.model();
  std::map<auto_parse::Action, auto_parse::Train_forecast_linear> all_training;
  for(auto_parse::Action a: auto_parse::all_actions)
    all_training[a] = auto_parse::Train_forecast_linear(lr_model.forecast(a),sampling_rate);
  std::map<Action, double> all_contrasts;
  double all_abs =  0;
  int number_to_read = end - begin;

  // make sure only read_only code is called since this is shared
  const auto_parse::Contrast contrast(parser, likelihood, feature_generator);

#pragma omp parallel default(shared)
  {
    std::map<auto_parse::Action, auto_parse::Train_forecast_linear> training = all_training;
    std::map<Action, double> contrasts = all_contrasts;
    double abs = all_abs;

#pragma omp for 
    for(int counter = 0; counter < number_to_read; ++counter)
      {
	const auto_parse::Words& sentence = *(begin + counter);
	std::vector<auto_parse::Row> contrast_pair = contrast(sentence);
	for(auto i = contrast_pair.begin(); i != contrast_pair.end(); ++i)
	  {
	    training[i->m_a](i->m_X, i->m_Y);
	    contrasts[i->m_a] += i->m_Y;
	    abs += std::abs(i->m_Y)/contrast_pair.size();
	  }
      };
#pragma omp critical
    for(auto_parse::Action a: auto_parse::all_actions)
      {
	all_training[a].merge(training[a]);
	all_contrasts[a] += contrasts[a];
      }
    all_abs += abs;
  }

  // Friendly output
  ostrm << std::endl;
  for(auto_parse::Action a: auto_parse::all_actions)
    ostrm << int(100.*all_contrasts[a]/number_to_read)/100. << " = " << a << "'s average value in a contrast." << std::endl;
  ostrm << "Typical deviation from zero is:" << round(100. * all_abs/number_to_read)/100. << std::endl;
  ostrm << std::endl;
    
  auto_parse::Model new_model = parser.model();
  for(auto_parse::Action a : auto_parse::all_actions)
    new_model.add_forecast(a,all_training[a].result());
  return new_model;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Likelihood
auto_parse::model_to_likelihood(const Eigenwords& parent,const Eigenwords& child,
				const auto_parse::Statistical_parse& parser,
				double scaling,
				std::vector<auto_parse::Words>::const_iterator begin,
				std::vector<auto_parse::Words>::const_iterator end)
{
  auto_parse::TP_eigenwords left(parent,child,scaling); 
  auto_parse::TP_eigenwords right(parent,child,scaling); 
  auto_parse::TP_iid         root(child.lexicon().size(),scaling); 
  auto_parse::Maximum_likelihood mle(left,right,root);
#pragma omp parallel default(shared)
  {
    auto thread_mle = mle;
    int number_to_read = end - begin;
#pragma omp for 
    for(int counter = 0; counter < number_to_read;++counter)
      {	
	const auto_parse::Words& sentence = *(begin + counter);
	auto d = redo_parse(sentence,parser(sentence)).parse();
	thread_mle(d);  // adds to database
      }
#pragma omp critical
    mle.merge(thread_mle);
  }

  Likelihood likelihood = mle.output();
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
  int number_to_read = end - begin;
  double log_like = 0;
  double total_left_links = 0;
  double total_links = 0;
  double total_stack_depth = 0;
  Eigen::VectorXd  pieces = Eigen::VectorXd::Zero(6);
#pragma omp parallel
  {
    Eigen::VectorXd piece = pieces;  // make a local copy
    
#pragma omp for reduction(+:log_like,total_links,total_left_links, total_stack_depth)
      for(int counter = 0; counter < number_to_read;++counter)
	{
	  const auto_parse::Words& sentence = *(begin + counter);
	  History h = parser.best_parse(sentence);  // by using best_parse here we can compare noisy methods with noise free methods.
	  total_stack_depth += h.maximum_stack_size();
	  auto_parse::Dependency parse = redo_parse(sentence, h).parse();
	  int n = sentence.end() - sentence.begin();
	  double prob = likelihood(parse) / n;
	   piece += likelihood.pieces(parse);
	   total_left_links += parse.number_left_links();
	   log_like += prob;
	   total_links += n;
	};
#pragma omp critical
      pieces += piece; // update global copy
  }
  pieces = pieces / number_to_read;
  std::stringstream summary;
  summary << "(old L.L. " << log_like/number_to_read
	  << ") on " << number_to_read/1000
	  << "k with " << round(100. * total_left_links / total_links) << "\\% lefts" << std::endl
	  << "new L.L. " << likelihood.summarize_pieces(pieces)
	  << "\naverage stack size = " << total_stack_depth/number_to_read;

  if(which_sentences.size() > 0)
    {
      std::vector<auto_parse::Dependency> parses;
      for(int sentence_id : which_sentences)
	parses.push_back(redo_parse(*(begin + sentence_id), parser.best_parse(*(begin + sentence_id))).parse());
      latex << "\\newpage\n\\section*{\\bf{" << rounds << ":}  " << summary.str() << "}\n\n" << std::endl;
      for(auto_parse::Dependency p : parses)
	likelihood.decorate(p, dictionary).latex(latex);
      debugging << likelihood.decorate(parses[0],dictionary);
    };
  return summary.str();
}
	
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Parse_args::Parse_args(int argc, char** argv)
  :
  sentence_file(), eigen_file(), latex_prefix(), gram_number(0), repeats_per_level(0),
  update_rate(0),scaling(0), noise(0), use_eager(false), r2l(false), comment()
{
  namespace po = boost::program_options;

  std::vector<std::string> comment_vec;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message (see parse_argv in learn.cc)")
    ("corpus", po::value<std::string>(&sentence_file)->default_value("eng_only"), "corpus to read from")
    ("dictionary", po::value<std::string>(&eigen_file)->default_value("pretty.csv"), "dictionary to read from")
    ("gram_number", po::value<int>(&gram_number)->default_value(3), "gram number for dictionary")
    ("latex", po::value<std::string>(&latex_prefix)->default_value("learn.output"), "latex file to write to (both a FILENAME.log.tex and a FILENAME.final.tex will be written to.)")
    ("update_rate", po::value<double>(&update_rate)->default_value(.05), "rate we move towards new data")
    ("scaling", po::value<double>(&scaling)->default_value(.01), "importance of distance in the likelihood calculation")
    ("noise", po::value<double>(&noise)->default_value(.1), "how noisy the decision making should be. 0=best guess, 3=almost pure noise.")
    ("eager", po::value<bool>(&use_eager)->default_value(false), "use eager or standard parsing.")
    ("reverse", po::value<bool>(&r2l)->default_value(false), "use right to left parsing.")
    ("repeats_per_level", po::value<int>(&repeats_per_level)->default_value(100), "number of times to process at each size")
    ("comment,c", po::value<std::vector<std::string> >(&comment_vec)->multitoken(), "comment about job to help organize output")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help"))
    {
      std::cout << desc << "\n";
      exit(1);
    }
  for(std::string s : comment_vec)
    comment += s + " ";
};

std::string
auto_parse::Parse_args::friendly_message(const auto_parse::Eigenwords& dictionary,
					 const std::vector<auto_parse::Words>& corpus) const
{
  std::stringstream out;
  int dim = dictionary.dimension();
  out << "\n\n               " << comment  << "\n\n\n" << std::endl;
  out << "     --corpus = " << sentence_file << std::endl;
  out << " --dictionary = " << eigen_file << std::endl;
  out << "--gram_number = " << gram_number << std::endl;
  out << "      --latex = " << latex_prefix << std::endl;
  out << "--update_rate = " << update_rate << "    (" << 1 - update_rate << " * old_model + " << update_rate << " * new_model)" << std::endl;
  out << "    --scaling = " << scaling << "    (-(Y - Yhat)^2 + " << scaling << " * log(probability skip) )" << std::endl;
  out << "      --eager = " << use_eager << std::endl;
  out << "      --noise = " << noise << "    randomly pick best action with about accuracy noise level." << std::endl;
  out << "    --reverse = " << r2l   << " process R to L" << std::endl;
  out << "repeats_per_level= " << repeats_per_level << std::endl;
  out << "   --comment = " << comment << std::endl;
  out << "Read a dictionary of size: " << dictionary.size()<< " x " << dim << print_time("reading") << std::endl;
  out << "Training on " << corpus.size() << " sentence.    " << std::endl;
  out << "number threads = " << auto_parse::number_of_threads_used() << "." << std::endl;
  return out.str();
}


void
auto_parse::Parse_args::print_latex(time_t start_time,
				    const std::vector<auto_parse::Words>& corpus,
				    const auto_parse::Likelihood& likelihood,
				    const std::vector<int>& number_to_train_on,
				    const auto_parse::Eigenwords& dictionary,
				    const auto_parse::Statistical_parse& parser) const
{
  int num_seconds = time(0) - start_time;
  int num_minutes = num_seconds / 60;
  int num_hours = num_minutes / 60;
  int extra_minutes = num_minutes - 60 * num_hours;

  std::ofstream latex_final(latex_prefix + ".final.tex");  // open it here in case the program gets killed.
  auto_parse::latex_header(latex_final,latex_prefix);
  latex_final << "\\section*{" << comment << "}\n";
  latex_final << "Parameters:\n\\begin{itemize}";
  latex_final << "\\item corpus: \\verb\"" << sentence_file << "\"" << std::endl;
  latex_final << "\\item latex: \\verb\"" << latex_prefix << "\"" << std::endl;
  latex_final << "\\item update: " << update_rate << std::endl;
  latex_final << "\\item scaling: " << scaling << std::endl;
  latex_final << "\\item noise: " << noise << std::endl;
  latex_final << "\\item eager: " << use_eager << std::endl;
  latex_final << "\\item reverse: " << r2l << std::endl;
  latex_final << "\\item repeats: " << repeats_per_level << std::endl;
  latex_final << "\\item Trained on " << corpus.size() << " sentence.    " << std::endl;
  latex_final << "\\item Threads used: " << auto_parse::number_of_threads_used()  << std::endl;
  latex_final << "\\item Total time: " << num_seconds << "    (" << num_hours << ":" << extra_minutes  << " h:m)" << std::endl;
  latex_final  << "\\item Date: \\today \n" ;
  latex_final << "\\end{itemize}\\newpage" << std::endl;

  std::vector<int> which_sentences {3643, 2, 4, 10, 17, 26};
  evaluation(*(number_to_train_on.end()-1), std::cout, latex_final, dictionary, parser, likelihood, which_sentences,
	     begin(corpus), end(corpus));

    which_sentences = std::vector<int>();
    std::multimap<int,std::vector<auto_parse::Words>::const_iterator> size_to_index;
    for(std::vector<auto_parse::Words>::const_iterator i =  begin(corpus); i != end(corpus); ++i)
      size_to_index.insert(std::make_pair(i->size(), i));

    latex_final << "\n\\newpage\n\n";

    for(int sentence_size = 0; sentence_size != 25;++sentence_size)
      {
	if(sentence_size == 18)
	  latex_final << "\\scriptsize\n";
	if(size_to_index.count(sentence_size) >= 5)
	  {
	    auto i = size_to_index.lower_bound(sentence_size);
	    latex_final << "\\newpage\n\\subsection*{" << sentence_size -1 << " words  : ";
	    latex_final << "\\#" << i->second - begin(corpus);
	    auto s1 = *(i->second);
	    auto p1 = redo_parse(s1, parser.best_parse(s1)).parse();
	    ++i;
	    latex_final << " + \\#"  << i->second - begin(corpus);
	    auto s2 = *(i->second);
	    auto p2 = redo_parse(s2, parser.best_parse(s2)).parse();
	    ++i;
	    latex_final << " + \\#"  << i->second - begin(corpus);
	    auto s3 = *(i->second);
	    auto p3 = redo_parse(s3, parser.best_parse(s3)).parse();
	    ++i;
	    latex_final << " + \\#"  << i->second - begin(corpus);
	    auto s4 = *(i->second);
	    auto p4 = redo_parse(s4, parser.best_parse(s4)).parse();
	    latex_final << "}  " << std::endl;
	    if(r2l)
	      {
		likelihood.decorate(p1, dictionary).latex_reversed(latex_final);
		likelihood.decorate(p2, dictionary).latex_reversed(latex_final);
		likelihood.decorate(p3, dictionary).latex_reversed(latex_final);
		likelihood.decorate(p4, dictionary).latex_reversed(latex_final);
	      }
	    else
	      {
		likelihood.decorate(p1, dictionary).latex(latex_final);
		likelihood.decorate(p2, dictionary).latex(latex_final);
		likelihood.decorate(p3, dictionary).latex(latex_final);
		likelihood.decorate(p4, dictionary).latex(latex_final);
	      }
	  }
      }
    auto_parse::latex_footer(latex_final);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::string
auto_parse::print_time(const std::string& h)
{
  static time_t running_time = time(0);  // used for timing 
  std::stringstream s;
  s << "\t\t\t\t" << h << " time " << time(0) - running_time << " sec." << std::endl;
  running_time = time(0);
  return s.str();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

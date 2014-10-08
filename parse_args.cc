// -*- c++ -*-


#undef _GLIBCXX_DEBUG

#include "parse_args.h"

#include "assert.h"
#include <iostream>

//  program_options don't play nice with GLIBCXX_DEBUG

#undef _GLIBCXX_DEBUG
#include <boost/program_options.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

auto_parse::Parse_args::Parse_args(int argc, char** argv)
  :
  golden_file(), sentence_file(), eigen_file(), latex_prefix(), gram_number(0), repeats_per_level(0),
  update_rate(0),scaling(0), noise(0), use_eager(false), r2l(false), comment()
{
  start_time = time(0);  // used for final timing
  namespace po = boost::program_options;

  std::vector<std::string> comment_vec;

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message (see parse_argv in learn.cc)")
    ("corpus", po::value<std::string>(&sentence_file)->default_value("eng_only"), "corpus to read from")
    ("gold",   po::value<std::string>(&golden_file)->default_value(""), "Gold standard parses")
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


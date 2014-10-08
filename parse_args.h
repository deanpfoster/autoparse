// -*- c++ -*-

#ifndef INCLUDED_PARSE_ARGS
#define INCLUDED_PARSE_ARGS

#include <string>


namespace auto_parse
{
  class Parse_args
  {
  public:
    Parse_args(int argc, char** argv);  // constructs and packs all the variables into a bundle

    // DATA  (exposed to the world)
    std::string golden_file, sentence_file, eigen_file, latex_prefix;
    int gram_number, repeats_per_level;
    double update_rate,scaling, noise;
    bool use_eager, r2l;
    std::string comment;
    time_t start_time = time(0);  // used for final timing

  };
}


#endif


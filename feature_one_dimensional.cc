// -*- c++ -*-


#include "feature_one_dimensional.Template.h"


template class auto_parse::Feature_one_dimensional<auto_parse::Words_left>;
template class auto_parse::Feature_one_dimensional<auto_parse::Sentence_length>;
template class auto_parse::Feature_one_dimensional<auto_parse::Stack_size>;
template class auto_parse::Feature_one_dimensional<auto_parse::Distance_to<0> >;
template class auto_parse::Feature_one_dimensional<auto_parse::Distance_to<1> >;
template class auto_parse::Feature_one_dimensional<auto_parse::Distance_to<2> >;


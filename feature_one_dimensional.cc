// -*- c++ -*-


#include "feature_one_dimensional.Template.h"


template class auto_parse::Feature_one_dimensional<auto_parse::Words_left>;
template class auto_parse::Feature_one_dimensional<auto_parse::Sentence_length>;
template class auto_parse::Feature_one_dimensional<auto_parse::Stack_size>;
template class auto_parse::Feature_one_dimensional<auto_parse::Distance_to<0> >;
template class auto_parse::Feature_one_dimensional<auto_parse::Distance_to<1> >;
template class auto_parse::Feature_one_dimensional<auto_parse::Distance_to<2> >;

using auto_parse::Feature_one_dimensional;
using auto_parse::Squared;
using auto_parse::Interaction;
using auto_parse::Words_left;
using auto_parse::Sentence_length;
using auto_parse::Stack_size;

template class Feature_one_dimensional<Squared<Words_left> >;
template class Feature_one_dimensional<Squared<Sentence_length> >;
template class Feature_one_dimensional<Squared<Stack_size> >;

template class Feature_one_dimensional<Interaction<Words_left, Stack_size> >;
template class Feature_one_dimensional<Interaction<Sentence_length, Stack_size> >;
template class Feature_one_dimensional<Interaction<Words_left, Sentence_length> >;

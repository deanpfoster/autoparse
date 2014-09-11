// -*- c++ -*-


#include "feature_interaction.Template.h"
#include "feature_one_dimensional.h"
#include "feature_eigenwords.h"

using auto_parse::Interaction;
using auto_parse::Words_left;
using auto_parse::Sentence_length;
using auto_parse::Stack_size;
using auto_parse::Feature_eigenwords;

typedef auto_parse::Feature_one_dimensional<auto_parse::Words_left> wl;
typedef auto_parse::Feature_one_dimensional<auto_parse::Sentence_length> sl;
typedef auto_parse::Feature_one_dimensional<auto_parse::Stack_size> ss;
// we don't need all 9, but this avoids having to know which one comes first
template class Interaction<wl, wl>;
template class Interaction<wl, ss>;
template class Interaction<wl, sl>;
template class Interaction<sl, sl>;
template class Interaction<sl, ss>;
template class Interaction<sl, wl>;
template class Interaction<ss, ss>;
template class Interaction<Interaction<ss, ss> , ss>;
template class Interaction<ss, wl>;
template class Interaction<ss, sl>;

typedef  auto_parse::Feature_eigenwords<auto_parse::Stack_top> t0;
typedef  auto_parse::Feature_eigenwords<auto_parse::Stack_1>   t1;
typedef  auto_parse::Feature_eigenwords<auto_parse::Stack_2>   t2;
typedef  auto_parse::Feature_eigenwords<auto_parse::Next_word> t3;

template class Interaction<t0, t0>;
template class Interaction<t0, t1>;

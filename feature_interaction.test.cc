//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include <fstream>
#include <iterator>

#include "feature_interaction.h"
#include "feature_one_dimensional.h"
#include "eigenwords.h"
#include "feature_eigenwords.h"

typedef auto_parse::Feature_one_dimensional<auto_parse::Words_left> wl;
typedef auto_parse::Feature_one_dimensional<auto_parse::Sentence_length> sl;
typedef auto_parse::Feature_one_dimensional<auto_parse::Stack_size> ss;
typedef auto_parse::Interaction<wl, wl> I_wl_wl;
typedef auto_parse::Interaction<wl, sl> I_wl_sl;


namespace auto_parse
{
  void test_interaction()
  {
    std::cout << "\n\n\n\t\t\t (feature_)INTERACTION<T>  feature_INTERACTION<T>  feature_INTERACTION<T>\n\n\n"<< std::endl;
    {
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords g(in,5);  // testing construction
      Feature_eigenwords<Stack_top>  s(g);

      wl wl_object;
      sl sl_object;
      I_wl_wl s1(wl_object,wl_object);
      I_wl_sl s2(wl_object,sl_object);
      Interaction<Feature_eigenwords<Stack_top>, Feature_eigenwords<Stack_top> > s3(s,s);
      std::cout << s1.variable_names() << std::endl;
      std::cout << s2.variable_names() << std::endl;
      std::cout << s3.variable_names() << std::endl;
      std::cout << s1.dimension() << std::endl;
      std::cout << s2.dimension() << std::endl;
      std::cout << s3.dimension() << std::endl;
      std::cout << s1.name() << std::endl;
      std::cout << s2.name() << std::endl;
      std::cout << s3.name() << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_interaction();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


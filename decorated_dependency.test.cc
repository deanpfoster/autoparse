//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <fstream>
#include "decorated_dependency.h"

namespace auto_parse
{
  void test_decorated_dependency()
  {
    std::cout << "\n\n\n\t\t\t DECORATED_DEPENDENCY  DECORATED_DEPENDENCY  DECORATED_DEPENDENCY\n\n\n"<< std::endl;
    {
      typedef auto_parse::Dependency D;
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      D complex =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
	< ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,"."));
      std::ifstream in("pretty_5_c_sample.csv");
      auto_parse::Eigenwords dictionary(in,5);  // testing construction
      Decorated_dependency pretty(complex, dictionary);
      Links links = pretty.links();
      for(Link link : links)
	{
	  pretty.describe_link(link, "prob");
	};
      pretty.describe_root("r00t");
      std::cout << pretty;
      pretty.latex(std::cout);
    }	
  }
}

#ifndef NO_MAIN
int
main()
{
 auto_parse::test_decorated_dependency();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


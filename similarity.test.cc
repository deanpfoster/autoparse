//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "interval.h"
#include "similarity.h"

namespace auto_parse
{
  void test_interval()
  {
    std::cout << "\n\n\n\t\t\t INTERVAL  INTERVAL  INTERVAL\n\n\n"<< std::endl;
    {
      typedef auto_parse::Dependency D;
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      D complex =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
	< ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,"."));
      std::cout << similarity(complex, complex);
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_interval();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


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
      D complex =  (D(Word(l,"A")) < D(Word(l,"hearing")) > (D(Word(l,"on")) > (D(Word(l,"the")) < D(Word(l,"issue")))))
	< ((D(Word(l,"is")) > (D(Word(l,"scheduled")) > D(Word(l,"today")))) > D(Word(l,".")));
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


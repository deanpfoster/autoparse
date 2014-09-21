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
      D complex =  (D(Word("A")) < D(Word("hearing")) > (D(Word("on")) > (D(Word("the")) < D(Word("issue")))))
	< ((D(Word("is")) > (D(Word("scheduled")) > D(Word("today")))) > D(Word(".")));
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


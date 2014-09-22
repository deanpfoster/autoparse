//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "interval.h"

namespace auto_parse
{
  void test_interval()
  {
    std::cout << "\n\n\n\t\t\t INTERVAL  INTERVAL  INTERVAL\n\n\n"<< std::endl;
    {
      std::cout << Interval(0,1).cross_check(Interval(0,1));
      std::cout << Interval(0,1).cross_check(Interval(1,2));
      std::cout << Interval(0,1).cross_check(Interval(2,3));
      std::cout << Interval(0,2).cross_check(Interval(1,3));
      std::cout << Interval(0,2).cross_check(Interval(1,2));
      std::cout << Interval(0,2).cross_check(Interval(0,1));
      std::cout << Interval(0,3).cross_check(Interval(1,2));
      std::cout << Interval(1,2).cross_check(Interval(0,1));
      std::cout << Interval(1,2).cross_check(Interval(0,2));
      std::cout << Interval(1,2).cross_check(Interval(0,3));
      std::cout << Interval(1,3).cross_check(Interval(0,2));
      std::cout << Interval(1,3).cross_check(Interval(0,1));
      std::cout << Interval(2,3).cross_check(Interval(0,1));

      typedef auto_parse::Dependency D;
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      D complex =  (D(Word(l,"A")) < D(Word(l,"hearing")) > (D(Word(l,"on")) > (D(Word(l,"the")) < D(Word(l,"issue")))))
	< ((D(Word(l,"is")) > (D(Word(l,"scheduled")) > D(Word(l,"today")))) > D(Word(l,".")));
      auto_parse::Intervals i1(complex);
      auto_parse::Intervals i2(complex);
      // note: i1 = i2.  So there shouldn't be any crosses
      std::cout << i1.count_crosses(i2) << std::endl;
      assert(i1.count_crosses(i2) == 0.0);
      std::cout << "constructed!" << std::endl;
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


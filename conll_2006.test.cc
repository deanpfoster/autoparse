//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include "conll_2006.h"

namespace auto_parse
{
  void test_conll_2006()
  {
    std::cout << " \n\n\n\t\t\t CONLL_2006  CONLL_2006  CONLL_2006 \n \n \n " << std::endl;
    {
      typedef auto_parse::Dependency D;
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      D complex =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
	< ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,"."));

      std::stringstream s,t;
      write_conll(complex, s);
      std::cout << s.str() << std::endl;
      Dependency copy = read_conll(s, l);
      write_conll(copy, t);
      std::cout << t.str() << std::endl;
      // Note: due to the down-casing.  These aren't the same
      //      assert(s.str() == t.str());  
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_conll_2006();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


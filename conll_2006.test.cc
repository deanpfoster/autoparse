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
      auto_parse::Conll_2006 write_only("conll_2066.test.cc",&l);
      write_only.write_parse(complex, s);
      std::cout << s.str() << std::endl;
      auto_parse::Conll_2006 conll(s,&l);
      Dependency copy = conll.next_sentence();
      conll.write_parse(copy, t);
      assert(s.str() == t.str());  // we can read what we write at least!
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


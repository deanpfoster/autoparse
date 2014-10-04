//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <fstream>
#include "gold_standard.h"
#include "dependency.h"

namespace auto_parse
{
  void test_gold_standard()
  {
    std::cout << "\n\n\n\t\t\t GOLD_STANDARD  GOLD_STANDARD  GOLD_STANDARD\n\n\n"<< std::endl;
    typedef auto_parse::Dependency D;
    Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
    D complex =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
      < ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,"."));

    D e1 =  D(l,"A") < (D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue")))
			< ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,".")));

    D error =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
      < ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) < D(l,"."));

    D errors =  (D(l,"A") > D(l,"hearing") > (D(l,"on") < (D(l,"the") < D(l,"issue"))))
      < ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) < D(l,"."));

    std::cout << complex;
    std::cout << e1;
    std::cout << error;
    std::cout << errors;
      
    Gold_standard g(complex);
    std::cout << "difference " << g(complex) << std::endl;
    std::cout << "difference " << g(e1) << std::endl;
    std::cout << "difference " << g(error) << std::endl;
    std::cout << "difference " << g(errors) << std::endl;

  }
}

#ifndef NO_MAIN
int
main()
{
 auto_parse::test_gold_standard();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


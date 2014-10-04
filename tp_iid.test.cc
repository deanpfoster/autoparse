//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "tp_iid.h"
#include "eigenwords.h"

namespace auto_parse
{
  void test_tp_iid()
  {
    std::cout << "\n\n\n\t\t\t TP_IID  TP_IID  TP_IID\n\n\n"<< std::endl;
    {
      Lexicon l {"<OOV>", "", "a", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      auto_parse::TP_iid tp(l.size(), .5);
      Words words(l,"");
      words.push_back(Word(l,"<OOV>"));
      words.push_back(Word(l,"the"));
      words.push_back(Word(l,"the"));
      Words::const_iterator a = words.begin();
      Words::const_iterator b = words.begin();
      ++b;
      Words::const_iterator c = b;
      ++c;
      std::cout << tp(a,b,words) << std::endl;
      std::cout << tp(b,a,words) << std::endl;
      std::cout << tp(b,c,words) << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_tp_iid();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "lr.h"

namespace auto_parse
{
  void test_LR()
  {
    std::cout << "\n\n\n\t\t\t LR  LR  LR\n\n\n"<< std::endl;
    {
      Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
      auto_parse::LR g(w);  // testing construction
      g.shift(); // A
      assert(g.next_input() == "hearing");
      g.shift(); // hearing
      assert(*(g.stack_top()) == "hearing");
      std::cout << g.parse();
      g.left_reduce();
      std::cout << g.parse();
      assert(*(g.stack_top()) == "hearing");
      g.shift(); // on
      assert(*(g.stack_2()) == "hearing");
      g.shift(); // the
      g.shift(); // issue
      g.left_reduce();
      g.right_reduce();
      g.right_reduce();  // stack = [hearing,S]
      assert(*(g.stack_top()) == "hearing");
      g.shift(); // is
      g.left_reduce();
      assert(*(g.stack_top()) == "is");
      g.shift(); // schedule
      g.shift(); // today
      g.right_reduce();
      g.right_reduce();
      g.shift(); // "."
      g.right_reduce();
      g.head_reduce();
      std::cout << g.parse();
      
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_LR();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


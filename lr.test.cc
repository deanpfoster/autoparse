//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <fstream>

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
      assert(*(g.stack(1)) == "hearing");
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
      
    };
    {
      Words w = Words() + "A" + "hearing"  + "is" + "scheduled" + "on" + "the" + "issue"+ "today" + ".";
      auto_parse::LR g(w);  // testing construction
      g.shift(); // A
      g.shift(); // hearing
      g.left_reduce();
      g.shift(); // is
      g.shift(); // schedule
      g.shift(); // on
      g.shift(); // the
      g.shift(); // issue
      g.left_reduce();
      g.right_reduce();
      g.right_cross_reduce(3);  // stack = [hearing,S]
      g.shift(); // today
      g.right_reduce();
      g.right_reduce();
      g.left_reduce();
      g.shift(); // "."
      g.right_reduce();
      g.head_reduce();
      std::cout << g.parse();

      std::ofstream latex("lr.test.tex");
      latex << "\\documentclass{article}\n\\usepackage{tikz-dependency}\n\\begin{document}\n";
      latex << "Original sentence--which has crosses in it:\n\n" << std::endl;
      latex << "\\begin{dependency}[theme = simple]\
                \\begin{deptext}[column sep=1em]					\
                 A \\& hearing \\& is \\& scheduled \\& on \\& the \\& issue \\& today \\& . \\\\ \
                \\end{deptext}\
                \\deproot{3}{ROOT}\
                \\depedge{2}{1}{ATT}\
                \\depedge[edge start x offset=-6pt]{2}{5}{ATT}\
                \\depedge{3}{2}{SBJ}\
                \\depedge{3}{9}{PU}\
                \\depedge{3}{4}{VC}\
                \\depedge{4}{8}{TMP}\
                \\depedge{5}{7}{PC}\
                \\depedge[arc angle=50]{7}{6}{ATT}\
                \\end{dependency}\n\n\n";
      latex << "So we have to use a skip LR parser:\n\n";
      g.parse().latex(latex);
      latex << "\\end{document}" << std::endl;

      
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


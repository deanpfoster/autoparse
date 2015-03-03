//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <fstream>
#include "dependency.h"

namespace auto_parse
{
  void test_Dependency()
  {
    std::cout << "\n\n\n\t\t\t DEPENDENCY  DEPENDENCY  DEPENDENCY\n\n\n"<< std::endl;
    {
      Lexicon l {"<OOV>", "four", "five", "six"};
      auto_parse::Dependency a(l,"four");  // testing construction
      std::cout << a << std::endl;
      auto_parse::Dependency b(l,"five"); 
      auto_parse::Dependency c(l,"six"); 
      auto_parse::Dependency d(a,auto_parse::Right_arrow(),b);  // testing construction
      std::cout << d << std::endl;
      auto_parse::Dependency e(d,auto_parse::Left_arrow(),c);  // testing construction
      std::cout << e << std::endl;
      
      std::cout << "constructed!" << std::endl;
      std::cout << e;
    };
    {
      typedef auto_parse::Dependency D;
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      D complex =  (D(l,"A") < D(l,"hearing") > (D(l,"on") > (D(l,"the") < D(l,"issue"))))
	< ((D(l,"is") > (D(l,"scheduled") > D(l,"today"))) > D(l,"."));
      std::cout << complex << std::endl;
      std::ofstream latex("dependency.test.tex");
      auto_parse::latex_header(latex);
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
      latex << "The best we could do since we don't do crosses:\n\n";
      complex.latex(latex);
      latex << "And now backwards:\n\n";
      complex.latex_reversed(latex);
      latex_footer(latex);
     
    }	
    {
      // Partial parsing example
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      Words w = Words(&l) + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
      auto_parse::Dependency d(w);
      assert(!d.full_parse());
      d.set_root(5);  // Zero based indexing
      assert(!d.full_parse());
      d.add(0,Left_arrow(),1);
      assert(!d.full_parse());  // Note: this check is redundent since it is checked by add()
      d.add(3,Left_arrow(),4);
      d.add(2,Right_arrow(),4);

      // checking what a partial parse print out looks like
      std::cout << d << std::endl;

      d.add(1,Right_arrow(),2);
      d.add(6,Right_arrow(),7);
      d.add(5,Right_arrow(),6);
      d.add(5,Right_arrow(),8);
      d.add(1,Left_arrow(),5);
      std::cout << d << std::endl;
      auto_parse::Dependency copy(d);
      assert(d.full_parse());
      assert(copy.full_parse());
      std::cout << "Number left links: " << d.number_left_links() << std::endl;
      auto is_pointer = d.sentence().begin() + 5;
      for(auto link : d.links())
	std::cout << link.parent()->convert_to_string(l) << " -> " << link.child()->convert_to_string(l) << std::endl;
      std::cout << "rightmost of " << is_pointer->convert_to_string(l) << " = " << d.right_most_child(is_pointer)->convert_to_string(l) << std::endl;
      std::cout << "leftmost of " << is_pointer->convert_to_string(l) << " = " << d.left_most_child(is_pointer)->convert_to_string(l) << std::endl;
    }
  }
}

#ifndef NO_MAIN
int
main()
{
 auto_parse::test_Dependency();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


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
      Word wa = "four";
      Word wb = "five";
      Word wc = "six";
      auto_parse::Dependency a(wa);  // testing construction
      auto_parse::Dependency b(wb);  // testing construction
      auto_parse::Dependency c(wc);  // testing construction
      auto_parse::Dependency d(a,auto_parse::Right_arrow(),b);  // testing construction
      auto_parse::Dependency e(d,auto_parse::Left_arrow(),c);  // testing construction
      
      std::cout << "constructed!" << std::endl;
      std::cout << e;
    };
    {
      typedef auto_parse::Dependency D;
      D complex =  (D("A") < D("hearing") > (D("on") > (D("the") < D("issue"))))
	< ((D("is") > (D("scheduled") > D("today"))) > D("."));
      std::cout << complex;
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
      latex << "The best we could do since we don't do crosses:\n\n" 
	    << complex;
      latex_footer(latex);
     
    }	
    {
      // Partial parsing example
      Words w = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
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


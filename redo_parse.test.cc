//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "redo_parse.h"

namespace auto_parse
{
  void test_redo_parse()
  {
    std::cout << "\n\n\n\t\t\t REDO_PARSE  REDO_PARSE  REDO_PARSE\n\n\n"<< std::endl;
    {
      Lexicon l {"<OOV>", "A", "hearing", "on", "the", "issue", "is", "scheduled", "today", "."};
      Words sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";
      auto_parse::History h;
      std::cout << redo_parse(sentence, h);
      h.push_back(auto_parse::Action::shift); // A
      h.push_back(auto_parse::Action::shift); // hearing
      h.push_back(auto_parse::Action::left_reduce);
      h.push_back(auto_parse::Action::shift); // on
      h.push_back(auto_parse::Action::shift); // the
      h.push_back(auto_parse::Action::shift); // issue
      h.push_back(auto_parse::Action::left_reduce);
      h.push_back(auto_parse::Action::right_reduce);
      std::cout << redo_parse(sentence, h);
      h.push_back(auto_parse::Action::right_reduce);  // stack = [hearing,S]
      h.push_back(auto_parse::Action::shift); // is
      h.push_back(auto_parse::Action::left_reduce);
      h.push_back(auto_parse::Action::shift); // schedule
      h.push_back(auto_parse::Action::shift); // today
      h.push_back(auto_parse::Action::right_reduce);
      h.push_back(auto_parse::Action::right_reduce);
      h.push_back(auto_parse::Action::shift); // "."
      h.push_back(auto_parse::Action::right_reduce);
      h.push_back(auto_parse::Action::head_reduce);

      std::cout << redo_parse(sentence, h);
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_redo_parse();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


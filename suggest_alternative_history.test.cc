//   -*- c++ -*-


#include <iostream>
#include <assert.h>

#include "suggest_alternative_history.h"
#include "utilities/z.h"
#define REPRODUCIBLE
#include "utilities/z.Template.h"
#include "word.h"

auto_parse::Value_of_forecasts randomize()
{
  auto_parse::Value_of_forecasts result;
  for(auto_parse::Action a: auto_parse::all_actions)
    result[a] = my_random::U();
  return result;
}

namespace auto_parse
{
  void test_suggest_alternative_prefix()
  {
    std::cout << "\n\n\n\t\t\t SUGGEST_ALTERNATIVE_PREFIX  SUGGEST_ALTERNATIVE_PREFIX  SUGGEST_ALTERNATIVE_PREFIX\n\n\n"<< std::endl;
    {
      Words sentence = Words() + "A" + "hearing" + "on" + "the" + "issue" + "is" + "scheduled" + "today" + ".";


      auto_parse::Statistical_history h;

      h.push_back(auto_parse::Action::shift,randomize()); // A
      h.push_back(auto_parse::Action::shift,randomize()); // hearing
      h.push_back(auto_parse::Action::left_reduce,randomize());
      h.push_back(auto_parse::Action::shift,randomize()); // on
      h.push_back(auto_parse::Action::shift,randomize()); // the
      h.push_back(auto_parse::Action::shift,randomize()); // issue
      h.push_back(auto_parse::Action::left_reduce,randomize());
      h.push_back(auto_parse::Action::right_reduce,randomize());
      h.push_back(auto_parse::Action::right_reduce,randomize());
      h.push_back(auto_parse::Action::shift,randomize()); // is
      h.push_back(auto_parse::Action::left_reduce,randomize());
      h.push_back(auto_parse::Action::shift,randomize()); // schedule
      h.push_back(auto_parse::Action::shift,randomize()); // today
      h.push_back(auto_parse::Action::right_reduce,randomize());
      h.push_back(auto_parse::Action::right_reduce,randomize());
      h.push_back(auto_parse::Action::shift,randomize()); // "."
      h.push_back(auto_parse::Action::right_reduce,randomize());
      h.push_back(auto_parse::Action::head_reduce,randomize());

      std::cout << h << std::endl;
      std::cout << suggest_alternative_history(h) << std::endl;
      std::cout << "code ran!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_suggest_alternative_prefix();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


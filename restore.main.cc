//   -*- c++ -*-

#include "likelihood.h"
#include "tokenize.h"
#include "tp_eigenwords.h"
#include "statistical_parse.h"
#include "redo_parse.h"

// Needed for "model_to_likelihood()" and "evalutation()"
#include "learn.h"

#define REPRODUCIBLE
// #define TRULY_RANDOM
#include "utilities/z.Template.h"
#include "utilities/iostream_box.h"

int
main(int,char** )
{
  Eigen::initParallel();  // this will help eigen not walk on top of itself.

  std::string restore_file = "restore.data";
  std::string latex_prefix = "latex.output";
  std::string sentence_file = "eng_only";
  std::string eigen_file = "pretty.csv";
  double scaling = .001;  // this might best be saved as part of something or other?
  int gram_number = 3;

  std::ofstream latex_final(latex_prefix + ".final.tex");
      
  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    Read in the Eigendictionary
  //
  //       (we shouldn't need it)
  //
  //////////////////////////////////////////////////////////////////////////////////

  std::ifstream in(eigen_file);
  auto_parse::Eigenwords dictionary(in,gram_number); 
  auto_parse::Eigenwords parent_dictionary(dictionary); 
  auto_parse::Eigenwords child_dictionary = dictionary.with_constant_row_sum_squares(); 

  //////////////////////////////////////////////////////////////////////////////////
  //
  //                    SET UP THE CRUDE TOKENIZER
  //
  //////////////////////////////////////////////////////////////////////////////////

  auto_parse::Tokenize corpus(sentence_file);
  std::vector<auto_parse::Words> corpus_in_memory;
  corpus.reset();
  while(!corpus.eof())
    corpus_in_memory.push_back(corpus.next_sentence());

  ///////////////////////////////////////////////////////////////////
  //
  //  This is the crucial step!  It reads the parser back in
  //
  ///////////////////////////////////////////////////////////////////

  auto_parse::Statistical_parse parser(std::cin);
  auto_parse::TP_eigenwords tp_left(dictionary,dictionary,scaling);  
  auto_parse::TP_eigenwords tp_right(dictionary,dictionary,scaling);  
  auto_parse::TP_eigenwords tp_root(auto_parse::Eigenwords::create_root_dictionary(),dictionary,scaling);  

  auto_parse::Likelihood likelihood(tp_left,tp_right,tp_root);
  likelihood = model_to_likelihood(parent_dictionary, child_dictionary, parser, scaling, begin(corpus_in_memory), end(corpus_in_memory));


  ///////////////////////////////////////////////////////////////////
  //
  //  Print some nice output.
  //
  ///////////////////////////////////////////////////////////////////

  auto_parse::latex_header(latex_final);
  latex_final << "Parameters:\n\\begin{itemize}";
  latex_final << "\\item corpus: \\verb\"" << sentence_file << "\"" << std::endl;
  latex_final << "\\item latex: \\verb\"" << latex_prefix << "\"" << std::endl;
  latex_final << "\\end{itemize}\\newpage" << std::endl;

  std::vector<int> which_sentences {3643, 2, 4, 10, 17, 26};
evaluation(0, std::cout, latex_final, dictionary, parser, likelihood, which_sentences, begin(corpus_in_memory), end(corpus_in_memory));

  which_sentences = std::vector<int>();
  std::multimap<int,std::vector<auto_parse::Words>::const_iterator> size_to_index;
  for(std::vector<auto_parse::Words>::const_iterator i =  begin(corpus_in_memory); i != end(corpus_in_memory); ++i)
    size_to_index.insert(std::make_pair(i->size(), i));

  latex_final << "\n\\newpage\n\n";

  for(int sentence_size = 0; sentence_size != 25;++sentence_size)
    {
      if(sentence_size == 18)
	latex_final << "\\scriptsize\n";
      if(size_to_index.count(sentence_size) >= 3)
	{
	  auto i = size_to_index.lower_bound(sentence_size);
	  latex_final << "\\subsection*{" << sentence_size -1 << " words  : \\#" << i->second - begin(corpus_in_memory) << " + " << std::endl;
	  auto s1 = *(i->second);
	  auto p1 = redo_parse(s1, parser(s1)).parse();
	  ++i;
	  latex_final << "\\#"  << i->second - begin(corpus_in_memory) << "}  " << std::endl;
	  auto s2 = *(i->second);
	  auto p2 = redo_parse(s2, parser(s2)).parse();
	  likelihood.decorate(p1, dictionary).latex(latex_final);
	  likelihood.decorate(p2, dictionary).latex(latex_final);
	}
    }
  auto_parse::latex_footer(latex_final);

}

//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>

#include "feature.h"

class Sample: public auto_parse::Feature
{
public:
  ~Sample(){};
  Sample(int length,double d)
    : m_fixed(length,d)
  {
    for(int i=0; i < length; ++i)
      m_fixed[i] = d + i;
  };

  std::vector<double>::iterator
  set_values(std::vector<double>::iterator i, const auto_parse::LR&) const
  {
    for(double x : m_fixed)
      {
	*i = x;
	++i;
      }
    return i;
  }

  std::vector<std::string>::iterator set_names(std::vector<std::string>::iterator result) const
  {
    std::string prefix = "Sample";
    for(unsigned int i = 0; i < m_fixed.size(); ++i)
      {
	std::stringstream s;
	s << prefix << "_" << i;
	*result = s.str();
	++result;
      }
    return result;
  }
  
  std::string name() const
  {
    std::stringstream s;
    s << "Sample(" << m_fixed.size() << "," << *m_fixed.begin() << "): An example feature";
    return s.str();
  }
  
  int dimension() const
  {
    return m_fixed.size();
  }
private:
  std::vector<double> m_fixed;

};


namespace auto_parse
{
  void test_feature()
  {
    std::cout << "\n\n\n\t\t\t FEATURE  FEATURE  FEATURE\n\n\n"<< std::endl;
    {
      Sample s(10,3.14159);
      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_feature();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


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
    : Feature(),
      m_fixed(length,d)
  {
    for(int i=0; i < length; ++i)
      m_fixed[i] = d + i;
  };

  Sample(const Sample&other)
    : Feature(other),
      m_fixed(other.m_fixed)
  {
  };

  Sample* clone() const
  {
    return new Sample(*this);
  };
  Sample* private_restore(std::istream&) const {return new Sample(*this);};
  auto_parse::Vector
  operator()(const auto_parse::LR&) const
  {
    auto_parse::Vector result(dimension());
    for(unsigned int i = 0; i <  m_fixed.size();++i)
      {
	result[i] = m_fixed[i];
      };
    return result;
  }

  std::vector<std::string> variable_names() const
  {
    std::string prefix = "Sample";
    std::vector<std::string> result(dimension());
    auto j = result.begin();
    for(unsigned int i = 0; i < m_fixed.size(); ++i)
      {
	std::stringstream s;
	s << prefix << "_" << i;
	*j = s.str();
	++j;
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


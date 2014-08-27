//   -*- c++ -*-

#include <iostream>
#include <assert.h>
#include <sstream>

#include "feature_generator.h"

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

  Sample(const Sample&other)
    : Feature(other),
      m_fixed(other.m_fixed)
  {
  };

  Sample* clone() const
  {
    return new Sample(*this);
  };

  Eigen::VectorXd
  operator()(const auto_parse::LR&) const
  {
    Eigen::VectorXd result(dimension());
    for(unsigned int i = 0; i <  m_fixed.size();++i)
      {
	result[i] = m_fixed[i];
      };
    return result;
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
  void test_feature_generator()
  {
    std::cout << "\n\n\n\t\t\t FEATURE_GENERATOR  FEATURE_GENERATOR  FEATURE_GENERATOR\n\n\n"<< std::endl;
    {
      Sample s(3,3.14159);
      Sample e(2,3.14159);
      Sample one(1,1);
      auto_parse::Feature_generator generator{&one,&e,&s};
      generator.print_on(std::cout);
      std::cout << "\nconstructed!" << std::endl;
    };
    // putting it here would be a pain in the butt.

  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_feature_generator();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


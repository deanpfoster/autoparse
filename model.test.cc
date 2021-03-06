//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include "model.h"
#include "forecast.h"
#define REPRODUCIBLE
#include "utilities/z.Template.h"

// See forcast.test.cc for comments on this class
class Sample: public auto_parse::Forecast
{
public:
  ~Sample(){};
  Sample(){};
  Sample(std::istream& in) :m_d()  {    in >> m_d >>  std::ws;  };
  Sample(double d):m_d(d){};
  Forecast* restore(std::istream& in) const  {    return new Sample(in);}
  Forecast* clone() const  {    return new Sample(m_d);}
  void tweak(const Forecast&,double)  {}
  void save(std::ostream& out) const  {out << m_d << std::endl; }
  std::string key() const {return "sample";};
  double operator()(const auto_parse::Vector&) const{ return m_d;}
private:
  double m_d;
};


namespace auto_parse
{
  void test_model()
  {
    std::cout << "\n\n\n\t\t\t MODEL  MODEL  MODEL\n\n\n"<< std::endl;

    Forecast::register_forecast(new Sample);
    {
      Sample example(10);
      Model m;
      for(Action a : standard_actions)
	{
	  m.add_forecast(a,example);
	};
      std::cout << "constructed via old fashioned methods!" << std::endl;
      Model cool(
      {   {Action::shift,&example},
	  {Action::left_reduce,&example},
	  {Action::right_reduce, &example},
	  {Action::head_reduce, &example}
      });


    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_model();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


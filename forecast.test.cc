//   -*- c++ -*-


#include <iostream>
#include <assert.h>
#include <sstream>
#include "forecast.h"

class Sample: public auto_parse::Forecast
{
public:
  // There needs to be a distructor since this is a virtual family
  ~Sample(){};

  // needs to be a default constructor for the restore object
  Sample(){};


  // needs to be a read from istream constructor for the restore
  Sample(std::istream& in)
    :m_d()
  {
    in >> m_d >>  std::ws;
  }

  // all other constructors are optional 
  Sample(double d):m_d(d){};

  // Needs to be the restore function itself.  They all look like this one.
  Forecast* restore(std::istream& in) const
  {
    return new Sample(in);
  }

  // Needs to be a save function.  What it writes should be read by the istream constructor
  void save(std::ostream& out) const
  {
    out << m_d << std::endl;
  }

  // This is basically the name of the class as a virtual function
  std::string key() const {return "sample";};
  
  // Finally!  The purpose of this class, namely make a prediction.
  double operator()(const auto_parse::LR&) const{ return m_d;}


private:
  double m_d;
  
};


namespace auto_parse
{

  struct LR{};

  void test_forecast()
  {
    std::cout << "\n\n\n\t\t\t FORECAST  FORECAST  FORECAST\n\n\n"<< std::endl;

    // Each type of forecast has to register itself.  This should be done in
    // the main program.  The object that is registed isn't collected at the end
    // and will show up as lost space in a good garbage collector.

    Forecast::register_forecast(Sample().key(), new Sample);
    {
      //      auto_parse::Forecast g;  // testing construction
      Sample example(10);
      std::stringstream s;
      s << example;  // save the example
      std::cout << example << std::endl;
      Forecast* p_forecast;
      s >> p_forecast;
      std::cout << example << std::endl; // confirm that it writes the same thing as before

      std::cout << "constructed!" << std::endl;
    };
  }
}

#ifndef NO_MAIN
int
main()
{
  auto_parse::test_forecast();  
  std::cout << "\n\nDONE." << std::endl;  
};
#endif


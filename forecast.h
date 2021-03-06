// -*- c++ -*-

#ifndef INCLUDED_FORECAST
#define INCLUDED_FORECAST

#include <iosfwd>
#include <map>
#include <Eigen/Core>
#include "eigenwords.h"

////////////////////////////////////////////////////////////////////////////////
//
// This is some good old fashioned object oriented tricks
//
//     a singleton to save sample forecast objects
//     the ability of a object of one type to create another object of that time
//     abstract base classes (Forecast is really a ABC)
//
// Look in the forecast.test.cc for how these tricks are used.
//
////////////////////////////////////////////////////////////////////////////////


namespace auto_parse
{
  class LR;  // forward reference so we don't actually need it.
  
  class Forecast
  {
  public:
    // CONSTRUCTORS
    static void register_forecast(const Forecast*);
    static std::map<std::string, const Forecast*> s_singletons;
    virtual ~Forecast();
    Forecast();
    virtual Forecast* restore(std::istream&) const = 0;
    virtual void save(std::ostream &) const = 0;
    virtual std::string key() const = 0;
    virtual Forecast* clone() const = 0;
    // MANIPULATORS
    virtual void tweak(const Forecast& other, double movement) = 0; // movement = 1 means replace old with new, movement=0 means use old

    // ACCESSORS
    virtual double operator()(const auto_parse::Vector&) const = 0;

  protected:
  private:
    Forecast(const Forecast &);            // Don't delete this.
    Forecast& operator=(const Forecast &); // Don't delete this.
  };
}

std::istream& operator>>(std::istream &, auto_parse::Forecast*&);
std::ostream& operator<<(std::ostream &,const auto_parse::Forecast &);

#endif

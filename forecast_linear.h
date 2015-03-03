// -*- c++ -*-

#ifndef INCLUDED_FORECAST_LINEAR
#define INCLUDED_FORECAST_LINEAR

#include <iosfwd>
#include <map>
#include "forecast.h"

////////////////////////////////////////////////////////////////////////////////
//
// The simplest forecast possible: return a fixed linear
//
////////////////////////////////////////////////////////////////////////////////


namespace auto_parse
{
  class Forecast_linear: public Forecast
  {
  public:
    // CONSTRUCTORS
    virtual ~Forecast_linear();
    Forecast_linear();
    Forecast_linear(std::istream&);
    Forecast_linear(const auto_parse::Vector&);
    Forecast_linear(const Forecast_linear &); 
    virtual Forecast* restore(std::istream&) const ;
    virtual void save(std::ostream &) const ;
    virtual std::string key() const;
    virtual Forecast_linear* clone() const;
    virtual void tweak(const Forecast& other, double movement);
    Forecast_linear& operator=(const Forecast_linear &);
    
    // MANIPULATORS
    // ACCESSORS
    double operator()(const auto_parse::Vector&) const ;
    int dimension() const {return m_weights.size();};

  protected:
  private:
    auto_parse::Vector m_weights;
  };
}

#endif

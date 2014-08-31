// -*- c++ -*-

#ifndef INCLUDED_FORECAST_CONSTANT
#define INCLUDED_FORECAST_CONSTANT

#include <iosfwd>
#include <map>
#include "forecast.h"

////////////////////////////////////////////////////////////////////////////////
//
// The simplest forecast possible: return a fixed constant
//
////////////////////////////////////////////////////////////////////////////////


namespace auto_parse
{
  class Forecast_constant: public Forecast
  {
  public:
    // CONSTRUCTORS
    virtual ~Forecast_constant();
    Forecast_constant();
    Forecast_constant(std::istream&);
    Forecast_constant(double);
    virtual Forecast* restore(std::istream&) const ;
    virtual void save(std::ostream &) const ;
    virtual std::string key() const;
    virtual Forecast_constant* clone() const;
    virtual void tweak(const Forecast&, double );

    // MANIPULATORS
    // ACCESSORS
    virtual double operator()(const Eigen::VectorXd&) const ;

  protected:
  private:
    double m_constant;
    Forecast_constant(const Forecast_constant &);            // Don't delete this.
    Forecast_constant& operator=(const Forecast_constant &); // Don't delete this.
  };
}

#endif

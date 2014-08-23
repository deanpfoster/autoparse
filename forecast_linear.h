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
    Forecast_linear(const Eigen::VectorXd&);
    virtual Forecast* restore(std::istream&) const ;
    virtual void save(std::ostream &) const ;
    virtual std::string key() const;

    // MANIPULATORS
    // ACCESSORS
    double operator()(const Eigen::VectorXd&) const ;

  protected:
  private:
    Eigen::VectorXd m_weights;
    Forecast_linear(const Forecast_linear &);            // Don't delete this.
    Forecast_linear& operator=(const Forecast_linear &); // Don't delete this.
  };
}

#endif

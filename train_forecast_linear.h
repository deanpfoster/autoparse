// -*- c++ -*-

#ifndef INCLUDED_TRAIN_FORECAST_LINEAR
#define INCLUDED_TRAIN_FORECAST_LINEAR

#include "forecast.h"

namespace auto_parse
{
  class Train_forecast_linear
  {
  public:
    // CONSTRUCTORS
    ~Train_forecast_linear();
    Train_forecast_linear(const Forecast&);  

    // MANIPULATORS
    void operator()(const Eigen::VectorXd&, double Y);
    // ACCESSORS
    Forecast_linear result() const;

  protected:
  private:
    Forecast_linear m_old_model;
    Eigen::MatrixXd m_XtX;  // X'X
    Eigen::VectorXd m_XtY;  // X'Y
  };
}
#endif

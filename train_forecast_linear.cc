// -*- c++ -*-


#include "train_forecast_linear.h"
#include "assert.h"
#include <iostream>
#include <Eigen/Dense>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Train_forecast_linear::~Train_forecast_linear()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear::Train_forecast_linear(const Forecast& f)
  :
  m_old_model(dynamic_cast<const auto_parse::Forecast_linear&>(f)),
  m_XtX(m_old_model.dimension(),m_old_model.dimension()),  // X'X
  m_XtY(m_old_model.dimension())   // X'Y
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear::Train_forecast_linear(const Train_forecast_linear & other)
  :
  m_old_model(other.m_old_model),
  m_XtX(other.m_XtX),
  m_XtY(other.m_XtY)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear&
auto_parse::Train_forecast_linear::operator=(const Train_forecast_linear & other)
{
  m_old_model = other.m_old_model;
  m_XtX = other.m_XtX;
  m_XtY =other.m_XtY;
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

void
auto_parse::Train_forecast_linear::operator()(const Eigen::VectorXd& X, double Y)
{
  m_XtX += X * (X.transpose());
  m_XtY += X * Y;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Forecast_linear
auto_parse::Train_forecast_linear::result() const
{
  Eigen::VectorXd beta = m_XtX.colPivHouseholderQr().solve(m_XtY);
  return Forecast_linear(beta);
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions


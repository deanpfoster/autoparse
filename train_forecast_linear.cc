// -*- c++ -*-


#include "train_forecast_linear.h"
#include "assert.h"
#include <iostream>
#include <Eigen/Dense>
#include "utilities/z.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Train_forecast_linear::~Train_forecast_linear()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear::Train_forecast_linear(const Forecast& f, double sampling_rate)
  :
  m_sample_size(0),
  m_sampling_rate(sampling_rate),
  m_old_model(dynamic_cast<const auto_parse::Forecast_linear&>(f)),
  m_XtX(Eigen::MatrixXd::Identity(m_old_model.dimension(),m_old_model.dimension())),  // X'X
  m_XtY(Eigen::VectorXd::Zero(m_old_model.dimension()))   // X'Y
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear::Train_forecast_linear()
  :
  m_sample_size(0),
  m_sampling_rate(-1),
  m_old_model(),
  m_XtX(),
  m_XtY()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear::Train_forecast_linear(const Train_forecast_linear & other)
  :
  m_sample_size(other.m_sample_size),
  m_sampling_rate(other.m_sampling_rate),
  m_old_model(other.m_old_model),
  m_XtX(other.m_XtX),
  m_XtY(other.m_XtY)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear&
auto_parse::Train_forecast_linear::operator=(const Train_forecast_linear & other)
{
  assert(&other != this);
  m_sample_size = other.m_sample_size;
  m_sampling_rate = other.m_sampling_rate;
  m_old_model = other.m_old_model;
  m_XtX = other.m_XtX;
  m_XtY =other.m_XtY;
  assert(m_sampling_rate == other.m_sampling_rate);  // we can't merge them otherwise
  return *this;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

void
auto_parse::Train_forecast_linear::operator()(const Eigen::VectorXd& X, double Y)
{
  assert(m_sampling_rate > 0);
  m_sample_size += 1;
  if(my_random::U_thread_safe() < m_sampling_rate)
    m_XtX += X * (X.transpose());
  m_XtY += X * Y;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::Train_forecast_linear::merge(const auto_parse::Train_forecast_linear& other)
{
  m_sample_size += other.m_sample_size;
  m_XtX += other.m_XtX;
  m_XtY += other.m_XtY;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
auto_parse::Forecast_linear
auto_parse::Train_forecast_linear::result() const
{
  //  Eigen::VectorXd beta = m_XtX.colPivHouseholderQr().solve(m_XtY);
  Eigen::MatrixXd inv = m_XtX.inverse();  // this version is paralizable
  Eigen::VectorXd beta = inv * m_XtY;
  Vector result = from_VectorXd(beta * m_sampling_rate);
  return Forecast_linear(result);
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


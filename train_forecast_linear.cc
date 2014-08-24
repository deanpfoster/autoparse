// -*- c++ -*-


#include "train_forecast_linear.h"
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Train_forecast_linear::~Train_forecast_linear()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Train_forecast_linear::Train_forecast_linear(const Forecast& f)
  :
  m_old_model(std::dynamic_cast<Forecast_linear&>(&f)),
  m_XtX,  // X'X
  m_XtY   // X'Y
{
  m_XtX = Eigen::MatrixXd(f.dimension(), f.dimension());
  m_XtY = Eigen::VectorXd(f.dimension());
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
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators
 

void
auto_parse::Train_forecast_linear::operator()(const Eigen::VectorXd& X, double Y)
{
  m_XtX += X * (X.transpose());
  mXtY += X * Y;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
void
auto_parse::Train_forecast_linear::result() const
{
  Eigen::MatrixXd inv = m_XtX.inverse();  // how do we compute this?
  Eigen::VectorXd result = inv * m_XtY;
  return result;
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

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Train_forecast_linear & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

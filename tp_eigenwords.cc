// -*- c++ -*-


#include "tp_eigenwords.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <math.h>
#include <Eigen/Dense>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using


////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::TP_eigenwords::~TP_eigenwords()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords::TP_eigenwords(const auto_parse::Eigenwords& parent,
					 const auto_parse::Eigenwords& child,
					 const Eigen::MatrixXd& XtX,
					 const Eigen::MatrixXd& XtY)
  :
  Transition_probability(),
  m_parent(parent),
  m_child(child),
  m_XtY(XtY),
  m_XtX(XtX)

{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords::TP_eigenwords(const auto_parse::Eigenwords& parent,const auto_parse::Eigenwords& child)
  : Transition_probability(),
    m_parent(parent),
    m_child(child),
    m_XtY(),
    m_XtX()
{
  m_XtY = Eigen::MatrixXd::Zero(parent.dimension(),child.dimension());
  m_XtX = 1 * Eigen::MatrixXd::Identity(parent.dimension(),parent.dimension()); // some regulirization
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords::TP_eigenwords(const auto_parse::TP_eigenwords& other)
  : Transition_probability(other),
    m_parent(other.m_parent),
    m_child(other.m_child),
    m_XtY(other.m_XtY),
    m_XtX(other.m_XtX)  
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords*
auto_parse::TP_eigenwords::clone() const
{
  return new auto_parse::TP_eigenwords(*this);
};
////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_eigenwords::accumulate(const Word& p, const Word& c) 
{
  const Eigen::VectorXd& pv = m_parent[p];
  const Eigen::VectorXd& cv = m_child[c];

  m_XtX += pv * (pv.transpose());  // outerproduct
  m_XtY += pv * (cv.transpose());  // outerproduct
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_eigenwords::merge(const Transition_probability& tp) 
{
  const TP_eigenwords& other = dynamic_cast<const TP_eigenwords&>(tp);
  m_XtY += other.m_XtY;
  m_XtX += other.m_XtX;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords*
auto_parse::TP_eigenwords::renormalize() const
{
  // Fast and parallel
  Eigen::MatrixXd inv = m_XtX.inverse();
  for(int i = 0; i < inv.rows(); ++i)
    for(int j = 0; j < inv.cols(); ++j)
      assert(!isnan(inv(i,j)));
  for(int i = 0; i < m_XtY.rows(); ++i)
    for(int j = 0; j < m_XtY.cols(); ++j)
      assert(!isnan(m_XtY(i,j)));
  Eigen::MatrixXd new_XtY = inv * m_XtY;

  //  more stable
  //  Eigen::VectorXd new_XtY = m_XtX.colPivHouseholderQr().solve(m_XtY);
  for(int i = 0; i < new_XtY.rows(); ++i)
    for(int j = 0; j < new_XtY.cols(); ++j)
      assert(!isnan(new_XtY(i,j)));

  // no NaN?
  //  Eigen::VectorXd new_XtY = m_XtY;
  Eigen::MatrixXd new_XtX = Eigen::MatrixXd::Identity(m_parent.dimension(),m_parent.dimension());
  return new TP_eigenwords(m_parent, m_child, new_XtX, new_XtY);
};


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
//
//  Likelihood calculation assumes that Y ~ N(X Beta, I).
//  So 2 * log(like) = |Y - X Beta|^2
//  This assumes that we have already computed X'X^-1 X'Y and shoved it in X'Y.
//
double
auto_parse::TP_eigenwords::operator()(const auto_parse::Word& parent, const auto_parse::Word& child) const
{
  const Eigen::VectorXd& p = m_parent[parent];
  const Eigen::VectorXd& c = m_child[child];
  Eigen::VectorXd prediction = p.transpose() * m_XtY;
  Eigen::VectorXd error = c - prediction;
  return - error.squaredNorm();
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::TP_eigenwords::print_on(std::ostream & ostrm) const
{
  ostrm << "Eigenword with transition matrix." << std::endl;
  double max = -1e100;
  double min = 1e100;
  double sum = 0;
  double sum2 = 0;
  for(int i = 0; i < m_XtY.rows(); ++i)
    for(int j = 0; j < m_XtY.cols(); ++j)
      {
	double v = m_XtY(i,j);
	double a = fabs(v);
	if(a > max)
	  max = a;
	if(a < min)
	  min = a;
	sum += v;
	sum2+= v * v;
      }
  ostrm << "\tmax = " << max << std::endl;
  ostrm << "\tmin = " << min << std::endl;
  double n = m_XtY.rows()*m_XtY.cols();
  ostrm << "\tmean= " << sum/n << std::endl;
  double var = sum2/n - (sum/n)*(sum/n);
  ostrm << "\t sd = " << sqrt(var) << std::endl;
  
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

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::TP_eigenwords & object)
{
  object.print_on(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

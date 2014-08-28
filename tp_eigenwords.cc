// -*- c++ -*-


#include "tp_eigenwords.h"

// put other includes here
#include "assert.h"
#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using


////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::TP_eigenwords::~TP_eigenwords()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords::TP_eigenwords(const auto_parse::Eigenwords& parent,const auto_parse::Eigenwords& child,
					 const Eigen::MatrixXd& t)
  :
  Transition_probability(),
  m_parent(parent),
  m_child(child),
  m_matrix(t)
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords::TP_eigenwords(const auto_parse::Eigenwords& parent,const auto_parse::Eigenwords& child)
  : Transition_probability(),
    m_parent(parent),
    m_child(child),
    m_matrix()  
{
  m_matrix = Eigen::MatrixXd(parent.dimension(),child.dimension());
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::TP_eigenwords::TP_eigenwords(const auto_parse::TP_eigenwords& other)
  : Transition_probability(other),
    m_parent(other.m_parent),
    m_child(other.m_child),
    m_matrix(other.m_matrix)  
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
  assert(pv.size() == m_matrix.rows());
  assert(cv.size() == m_matrix.cols());
  m_matrix += pv * (cv.transpose());  // outerproduct
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_eigenwords::merge(const Transition_probability& tp) 
{
  const TP_eigenwords& other = dynamic_cast<const TP_eigenwords&>(tp);
  m_matrix += other.m_matrix;
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void
auto_parse::TP_eigenwords::renormalize() 
{
  Eigen::VectorXd weights = m_matrix * Eigen::VectorXd::Ones(m_child.dimension());
  Eigen::VectorXd inverse_weights = weights.array().inverse();
  m_matrix = m_matrix * inverse_weights.asDiagonal();
};


////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
double
auto_parse::TP_eigenwords::operator()(const auto_parse::Word& parent, const auto_parse::Word& child) const
{
  const Eigen::VectorXd& p = m_parent[parent];
  const Eigen::VectorXd& c = m_child[child];
  double result = p.transpose() * m_matrix * c;
  result = fabs(result);
  return result;
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
  for(int i = 0; i < m_matrix.rows(); ++i)
    for(int j = 0; j < m_matrix.cols(); ++j)
      {
	double v = m_matrix(i,j);
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
  double n = m_matrix.rows()*m_matrix.cols();
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

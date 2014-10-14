// -*- c++ -*-

#ifndef INCLUDED_vector
#define INCLUDED_vector

// This file only has type def's and hence no testing code.
// It probably should have some testing code when it starts
// making a std::vector look like an Eigen::VectorXd

// #define AVOID_EIGEN

#ifdef AVOID_EIGEN
#include <vector>
#else
#include <Eigen/Core>
#endif

namespace auto_parse
{
  // Note:  typedef Matrix<double, Dynamic, Dynamic, RowMajor> Eigen::MatrixXd;
  // So using a MatrixXd is column major order which is
  // WRONG for accessing a row at a time in a fast fashion.
  // If we need to call serious Eigen code (i.e. SVD / LU) then it might be
  // worth while changing rows and columns around since Eigen code is setup
  // to use MatrixXd by default and so might run faster / better.
  //
  // Using the following might be better:
  //     #define EIGEN_DEFAULT_TO_ROW_MAJOR
  // than
  //    typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Matrix;
  // This assumes that I haven't found my memory leak!
  //

#ifdef AVOID_EIGEN
  typedef std::vector<double> Vector;
  typedef std::vector<std::vector<double> > Matrix;
  
  inline double norm(const Vector& vec)
  {
    double ss = 0;
    for(double x: vec)
      ss += x * x;
    return(sqrt(ss));
  }

  inline Eigen::VectorXd to_VectorXd(const Vector& vec)
  {
    Eigen::VectorXd result(vec.size());
    for(int i = 0; i < vec.size(); ++i)
      result(i) = vec[i];
    return result;
  }

#else
  typedef Eigen::MatrixXd Matrix;
  typedef Eigen::VectorXd Vector;
  inline double norm(const Vector& vec)
  {
    return vec.norm();
  }

  inline Eigen::VectorXd to_VectorXd(const Vector& vec)
  {
    return vec;
  }
#endif
}


#endif

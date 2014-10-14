// -*- c++ -*-

#ifndef INCLUDED_vector
#define INCLUDED_vector

// This file only has type def's and hence no testing code.
// It probably should have some testing code when it starts
// making a std::vector look like an Eigen::VectorXd

// #define AVOID_EIGEN

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
  typedef Eigen::MatrixXd Matrix;
  typedef Eigen::VectorXd Vector;
}


#endif

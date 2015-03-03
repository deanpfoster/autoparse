// -*- c++ -*-


#include "similarity.h"

#include "assert.h"
#include "interval.h"
#include "dependency.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

double
auto_parse::fraction_crosses(const auto_parse::Dependency& A,const auto_parse::Dependency& B)
{
  assert(&A.sentence() == &B.sentence());
  Intervals iA(A);
  Intervals iB(B);
  int total_crosses = iA.count_crosses(iB);
  double n = A.sentence().size();
  return total_crosses / n;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

double
auto_parse::similarity(const auto_parse::Dependency& A,const auto_parse::Dependency& B)
{
  return fraction_crosses(A,B);
}



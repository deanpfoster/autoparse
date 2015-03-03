// -*- c++ -*-

#ifndef INCLUDED_ROW
#define INCLUDED_ROW

#include <iosfwd>
#include "history.h"
#include <Eigen/Core>
#include <vector>
#include "word.h"

namespace auto_parse
{
  class Feature_generator;
  struct Row
  {
    Action m_a;
    double m_Y;
    Eigen::VectorXd m_X;
  };

  std::vector<Row> rows(const auto_parse::Feature_generator& features,
			const auto_parse::Words& w,
			const auto_parse::History& common,
			Action a1, double v1,
			Action a2, double v2);

  void write_row(std::ostream& out, const std::vector<auto_parse::Row>& rows) ;

};


std::ostream &
operator<<(std::ostream & , const std::vector<auto_parse::Row> & );




#endif

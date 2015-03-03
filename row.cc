// -*- c++ -*-


#include "row.h"
#include "assert.h"
#include <iostream>
#include "lr.h"
#include "redo_parse.h"
#include "feature_generator.h"

////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::vector<auto_parse::Row>
auto_parse::rows(const auto_parse::Feature_generator& features,
		 const auto_parse::Words& w,
		 const auto_parse::History& common,
		 Action a1, double v1,
		 Action a2, double v2) 
{
  std::vector<Row> result(2);
  LR parse_state = redo_parse(w, common);
  Eigen::VectorXd X = features(parse_state);
  double diff = (v1 - v2)/2;
  // a1/v1 row
  result[0].m_Y = diff;
  result[0].m_a = a1;
  result[0].m_X = X;
  // a2/v2 row
  result[1].m_Y = -diff;
  result[1].m_a = a2;
  result[1].m_X = X;
  return result;
};

void
auto_parse::write_row(std::ostream& out,
		      const std::vector<auto_parse::Row>& rows) 
{
  for(Row row : rows)
    {
      out << row.m_Y << ",\"" << row.m_a << "\"";
      for(int i = 0; i < row.m_X.size(); ++i)
	out << "," << row.m_X[i];
      out << std::endl;
    }
};

std::ostream &
operator<<(std::ostream & ostrm, const std::vector<auto_parse::Row> & object)
{
  auto_parse::write_row(ostrm, object);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

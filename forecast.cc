// -*- c++ -*-


#include "forecast.h"

// put other includes here
#include "assert.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
//                          U S I N G   D I R E C T I V E S                            using

using namespace auto_parse;  // Lazy if you leave this in. Try not to do: "using foo;"  It adds too much

std::map<std::string, const Forecast*> auto_parse::Forecast::s_singletons;


////////////////////////////////////////////////////////////////////////////////////////////
//                              C O N S T R U C T O R S                         constructors

auto_parse::Forecast::~Forecast()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast::Forecast()
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
auto_parse::Forecast::Forecast(const Forecast & ) 
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                             M A N I P U L A T O R S                          manipulators

////////////////////////////////////////////////////////////////////////////////////////////
//                               A C C E S S O R S                                 accessors
void
auto_parse::Forecast::save(std::ostream & ) const
{
};
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
auto_parse::Forecast::register_forecast(const std::string& key, const Forecast* p_value)
{
  assert(s_singletons[key] == 0);
  s_singletons[key] = p_value;
}

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R O T E C T E D                                     protected

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////////////////////////////////////////////////////////////////////////
//                           P R I V A T E                                           private


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
////////////////////////////////////////////////////////////////////////////////////////////
//                     F R E E   F U N C T I O N S                            free functions

std::istream & operator>>(std::istream & istrm,  auto_parse::Forecast*& p_object)
{
  std::string key;
  getline(istrm,key);
  assert(auto_parse::Forecast::s_singletons.find(key) != auto_parse::Forecast::s_singletons.end());
  p_object = auto_parse::Forecast::s_singletons.find(key)->second->restore(istrm);
  return istrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::ostream & operator<<(std::ostream & ostrm, const auto_parse::Forecast & object)
{
  ostrm << object.key() << std::endl;
  object.save(ostrm);
  return ostrm;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

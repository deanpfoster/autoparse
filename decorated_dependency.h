// -*- c++ -*-

#ifndef INCLUDED_DECORATED_DEPENDENCY
#define INCLUDED_DECORATED_DEPENDENCY

#include "dependency.h"
#include "eigenwords.h"

namespace auto_parse
{
  class Decorated_dependency: public Dependency
  {
  public:

    // CONSTRUCTORS
    ~Decorated_dependency();
    Decorated_dependency(const Dependency&, const Eigenwords&); 

    // MANIPULATORS
    void describe_link(const Link&, const std::string&);
    void describe_link(const Link&, double);  // does a courtesy conversation to string

  protected:
    std::string link_description(const Link&) const;
    std::string word_description(const Word&) const;
  private:
    Eigenwords m_dictionary;
    std::map<Link, std::string> m_description;
    

  };

};


#endif

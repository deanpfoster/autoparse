// -*- c++ -*-

#ifndef INCLUDED_FEATURE
#define INCLUDED_FEATURE

#include <iosfwd>
#include <vector>

namespace auto_parse
{
  class LR;
  class Feature
  {
  public:
    typedef std::vector<double>::iterator data_iterator;
    typedef std::vector<std::string>::iterator name_iterator;

    // CONSTRUCTORS
    virtual ~Feature();
    Feature();

    // MANIPULATORS
    // ACCESSORS

    // the following two are "visitors" which modifies the vector as it goes along
    virtual data_iterator set_values(data_iterator, const LR&) const = 0;  
    virtual name_iterator set_names(name_iterator) const = 0;
    virtual std::string   name() const = 0;
    virtual int           dimension() const = 0;

  protected:
  private:
    Feature(const Feature &);            // Don't delete this.
    Feature& operator=(const Feature &); // Don't delete this.
  };
}

#endif

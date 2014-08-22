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
    // CONSTRUCTORS
    virtual ~Feature();
    Feature();

    // MANIPULATORS
    // ACCESSORS

    // the following two are "visitors" which modifies the vector as it goes along
    virtual
      std::vector<double>::iterator
      set_values(std::vector<double>::iterator, const LR&) const = 0;  
    virtual
      std::vector<std::string>::iterator
      names(std::vector<std::string>::iterator) const = 0;
    virtual
      std::string name() const = 0;
    virtual
      int dimension() const = 0;

  protected:
  private:
    Feature(const Feature &);            // Don't delete this.
    Feature& operator=(const Feature &); // Don't delete this.
  };
}

#endif

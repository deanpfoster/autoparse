// -*- c++ -*-

#ifndef INCLUDED_GENERIC
#define INCLUDED_GENERIC

#include <iosfwd>

namespace auto_parse
{
  class Generic
  {
  public:
    // CONSTRUCTORS
    ~Generic();
    Generic();

    Generic* clone() const;
    // MANIPULATORS

    // ACCESSORS
    virtual void print_on(std::ostream &) const;

  protected:

  private:
    Generic(const Generic &);            // Don't delete this.
    Generic& operator=(const Generic &); // Don't delete this.
  };
}

std::ostream& operator<<(std::ostream &,const auto_parse::Generic &);

#endif

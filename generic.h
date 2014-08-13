// -*- c++ -*-

#ifndef INCLUDED_GENERIC
#define INCLUDED_GENERIC

#include <iosfwd>

namespace library
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

std::ostream& operator<<(std::ostream &,const library::Generic &);

#endif

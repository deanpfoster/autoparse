// -*- c++ -*-

#ifndef INCLUDED_FEATURE
#define INCLUDED_FEATURE

#include <iosfwd>
#include <vector>
#include <Eigen/Core>

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
    virtual Feature* clone() const = 0;
    // MANIPULATORS
    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR&) const;
    // the following use "visitor". SHould be changed to just return list
    virtual name_iterator set_names(name_iterator) const = 0;
    virtual std::string   name() const = 0;
    virtual int           dimension() const = 0;

  protected:
    Feature(const Feature &);            // Don't delete this.
  private:
    virtual data_iterator set_values(data_iterator, const LR&) const;  
    Feature& operator=(const Feature &); // Don't delete this.
  };
}

#endif

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

    // CONSTRUCTORS
    virtual ~Feature();
    Feature();
    Feature(std::istream&){};
    virtual Feature* clone() const = 0;
    static Feature* restore(std::istream&);
    // MANIPULATORS
    // ACCESSORS
    virtual Eigen::VectorXd operator()(const LR&) const = 0;
    virtual std::vector<std::string> variable_names() const = 0;
    virtual std::string   name() const = 0;
    virtual int           dimension() const = 0;

  protected:
    Feature(const Feature &);            // Don't delete this.
  private:
    virtual Feature* private_restore(std::istream&) const = 0;
    Feature& operator=(const Feature &); // Don't delete this.
    static void register_class(const std::string&, const Feature&);
  };
}

#endif

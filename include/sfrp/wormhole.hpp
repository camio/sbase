#ifndef SFRP_WORMHOLE_HPP_
#define SFRP_WORMHOLE_HPP_

//@PURPOSE: Provide a means to create circular dependencies in FRP graphs.
//
//@CLASSES:
//  sfrp::Wormhole: circular connection between behaviors
//
//@SEE_ALSO: sfrp_wormholeutil
//
//@DESCRIPTION: This component provides a single class, 'Wormhole', that allows
// one to make connections in a FRP dependency graph that are circular.
//
// Circular graphs are required, for instance, when one needs the previous
// state of the graph to be remembered. Consider how one would implement a
// 'withPrev' function that returns a behavior equivelent to its input
// behavior, except it pairs the value at every step with the previously
// pulled value.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Creating a 'withPrev' function
// - - - - - - - - - - - - - - - - - - - - -
// Below is the signature of the function we'd like to implement.
//
//..
//  template <typename T>
//  sfrp::Behavior<std::pair<T, T>> pmWithPrev(const T& initialValue,
//                                             const sfrp::Behavior<T>& b);
//..
// 
// The 'initialValue' is required in order to define what is paired with the
// first pull of 'b'.
//
// We start with the creation of our wormhole,
//..
//  Wormhole<T> w = Wormhole<T>(initialValue);
//..
// The wormhole, at this point, is in an "unclosed" state. Although we have
// access to its 'outputBehavior' method, it has yet to be defined what the
// input behavior is. The way we can use the 'outputBehavior' without defining
// what it actually is reason why wormholes are useful for the creation of
// circular dependencies.
//..
//  return pmZip(w.outputBehavior(), w.setInputBehavior(b));
//..
// Our function ends with zipping up the yet-to-be-defined output behavior with
// a curious call to 'setInputBehavior',
//
// 'setInputBehavior' both defines what the 'outputBehavior' of the wormhole is
// and creates a new behavior, equivelent to its argument, that ensures the
// wormhole is kept up-to-date. It is important that the return value of
// 'setInputBehavior' is pulled at every time of interest otherwise
// 'outputBehavior' will return values that are out of date.

#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/behavior.hpp>
#include <utility>  // std::pair

namespace sfrp {

// This class provides a utility that enables one to create circular
// dependencies between behaviors.
template <typename T>
struct Wormhole {
  typedef T type;

  // Create a new wormhole object that has an initial output of the specified
  // 'value'. The newly created wormhole is not closed.
  Wormhole(const T& value);

  // Set this wormhole's input to the specified 'inputBehavior'. Return a
  // behavior equivelent to 'inputBehavior', but which feeds this wormhole as a
  // side-effect of 'pull' operations. This wormhole is then set to closed. The
  // behavior is undefined unless 'closed() == false'.
  sfrp::Behavior<T> setInputBehavior(const Behavior<T>& inputBehavior) const;

  // Return 'true' if this wormhole is closed and 'false' otherwise.
  bool closed() const;

  // Return the output behavior corresonding to this wormhole.
  sfrp::Behavior<T> outputBehavior() const;

 private:
  template <typename T>
  friend struct Wormhole_ClosedBehaviorFunction;

  Wormhole(const boost::shared_ptr<std::pair<bool, boost::optional<T>>>& data_);

  boost::shared_ptr<std::pair<bool, boost::optional<T>>> m_data;

  sfrp::Behavior<T> m_outputBehavior;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Wormhole<T>::Wormhole(
    const boost::shared_ptr<std::pair<bool, boost::optional<T>>>& data_)
    : m_data(data_) {}

template <typename T>
bool Wormhole<T>::closed() const {
  return m_data->first;
}

template <typename T>
sfrp::Behavior<T> Wormhole<T>::outputBehavior() const
{
  return m_outputBehavior;
}

template <typename T>
struct Wormhole_BehaviorFunction {
  Wormhole_BehaviorFunction(
      const boost::shared_ptr<std::pair<bool, boost::optional<T>>>& data_)
      : data(data_) {}
  typedef boost::optional<T> result_type;
  result_type operator()(const double time) const { return data->second; }
  boost::shared_ptr<std::pair<bool, boost::optional<T>>> data;
};

template <typename T>
Wormhole<T>::Wormhole(const T& value)
    : m_data(boost::make_shared<std::pair<bool, boost::optional<T>>>(false,
                                                                     value)),
      m_outputBehavior(Behavior<T>::fromValuePullFunc(
          Wormhole_BehaviorFunction<T>(m_data))) {}

template <typename T>
struct Wormhole_ClosedBehaviorFunction {
  Wormhole_ClosedBehaviorFunction(const Wormhole<T>& wh_,
                                  const Behavior<T>& pm_)
      : wh(wh_), pm(pm_) {}
  typedef boost::optional<T> result_type;
  result_type operator()(const double time) const {
    return wh.m_data->second = pm.pull(time);
  }
  Wormhole<T> wh;
  Behavior<T> pm;
};

template <typename T>
Behavior<T> Wormhole<T>::setInputBehavior(const Behavior<T>& pm) const {
  m_data->first = true;
  return Behavior<T>::fromValuePullFunc(Wormhole_ClosedBehaviorFunction<T>(*this, pm));
}
}
#endif

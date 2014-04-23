#ifndef SFRP_INCREASINGPARTIALTIMEFUNCTION_HPP_
#define SFRP_INCREASINGPARTIALTIMEFUNCTION_HPP_

//@PURPOSE: Provide a partial time function class that releases resources.
//
//@CLASSES:
//  sfrp::IncreasingPartialTimeFunction: resource releasing time function class
//
//@SEE_ALSO: sfrp_cachedincreasingpartialtimefunction
//
//@DESCRIPTION: This component provides a single class,
// 'IncreasingPartialTimeFunction', which wraps a specially constrained function.
//
// The function supplied to 'IncreasingPartialTimeFunction's constructor is
// guarenteed to only be called with ever increasing time values. It is also
// guarenteed to no longer call that function after it first returns
// 'boost::none' indicating the end of the partial time definition.
//
// 'IncreasingPartialTimeFunction's 'pull()' function adds a couple features to
// the underlying time function. First, it can be called with increasing values
// of time even after 'boost::none' has been reached. Second, the first time
// 'boost::none' is read from the underlying function, all that underlying
// function's resources are freed up.
//
// Generally speaking, this class is intended for use as a tool to build up
// an implementation of the sfrp_behavior component.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Relax time function preconditions
// - - - - - - - - - - - - - - - - - - - - - -
// Say we have a function with the following signature and preconditions:
//..
// // The behavior is undefined unless the specified 'time' is greater than the
// // last call to this function. The behavior is also undefined unless the
// // previous call to 'timeFunction()' did not return 'boost::none'.
// boost::optional<int> timeFunction(double time);
//..
// We would like to remove the second precondition to this function. We can do
// that by wrapping it in an 'sfrp::IncreasingPartialTimeFunction' object.
//..
//  // The behavior is undefined unless the specified 'time' is greater than the
//  // last call to this function.
//  boost::optional<int> relaxedTimeFunction(double time) {
//    static sfrp::IncreasingPartialTimeFunction<int> wrappedTimeFunction(
//        timeFunction);
//    return wrappedTimeFunction.pull(time);
//  }
//..

#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace sfrp {

// This class implements a partial time function.
template <typename Value>
struct IncreasingPartialTimeFunction {
  // Create a new 'IncreasingPartialTimeFunction<Value>' object from the
  // specified 'valuePullFunc' function.  The behavior is undefined unless
  // 'valuePullFunc' returns a value when it is called once and is defined with
  // increasing argument values as long as the previous call didn't return
  // 'boost::none'.
  IncreasingPartialTimeFunction(
      boost::function<boost::optional<Value>(double)> valuePullFunc);

  // Create a new 'IncreasingPartialTimeFunction' object that has the same
  // value as the specified 'other' object.
  IncreasingPartialTimeFunction(IncreasingPartialTimeFunction&& other);

  // Return the value of this partial time function at the specified 'time' if
  // it is defined, otherwise return 'boost::none'. If a previous call to
  // 'pull()' returns 'boost::none', so will all subsequent calls. The first
  // time 'boost::none' is returned, the underlying function object is garbage
  // collected.
  //
  // The behavior is undefined unless 'pull()' is being called for the first
  // time or the previous call to 'pull()' used a time argument that is less
  // than 'time'.
  //
  // Note that the aformentioned preconditions imply that 'pull()' is called
  // with increasing time arguments and that after 'pull()' returns
  // 'boost::none', it will return 'boost::none' thereafter.
  boost::optional<Value> pull(const double time);

  // Give this 'IncreasingPartialTimeFunction' object the same value as the
  // specified 'other' object.
  IncreasingPartialTimeFunction& operator=(
      IncreasingPartialTimeFunction&& other);

 private:
  // not implemented
  IncreasingPartialTimeFunction(const IncreasingPartialTimeFunction&);
  IncreasingPartialTimeFunction& operator=(
      const IncreasingPartialTimeFunction&);

  boost::function<boost::optional<Value>(double)> m_valuePullFunc;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Value>
IncreasingPartialTimeFunction<Value>::IncreasingPartialTimeFunction(
    boost::function<boost::optional<Value>(double)> valuePullFunc)
    : m_valuePullFunc(valuePullFunc) {}

template <typename Value>
IncreasingPartialTimeFunction<Value>::IncreasingPartialTimeFunction(
    IncreasingPartialTimeFunction&& other) {
  m_valuePullFunc = std::move(other.m_valuePullFunc);
}

template <typename Value>
boost::optional<Value> IncreasingPartialTimeFunction<Value>::pull(
    const double time) {
  if (!m_valuePullFunc.empty()) {
    boost::optional<Value> result = m_valuePullFunc(time);
    if (!result) {
      m_valuePullFunc.clear();
    }
    return result;
  } else {
    return boost::none;
  }
}
template <typename Value>
IncreasingPartialTimeFunction<Value>& IncreasingPartialTimeFunction<Value>::operator=(
    IncreasingPartialTimeFunction&& other) {
  m_valuePullFunc = std::move(other.m_valuePullFunc);
}
}
#endif

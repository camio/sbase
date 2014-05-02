#ifndef SFRP_CACHEDINCREASINGPARTIALTIMEFUNCTION_HPP_
#define SFRP_CACHEDINCREASINGPARTIALTIMEFUNCTION_HPP_

//@PURPOSE: Provide a partial time function class that allows same-time pulls.
//
//@CLASSES:
//  sfrp::CachedIncreasingPartialTimeFunction: same-time pulling time function
//
//@SEE_ALSO: sfrp_increasingpartialtimefunction
//
//@DESCRIPTION: This component provides a single class,
// 'CachedIncreasingPartialTimeFunction', which wraps a specially contstrained
// function and add features to it.
//
// The function supplied to 'CachedIncreasingPartialTimeFunction's constructor
// is guarenteed to only be called with ever increasing time values. It is also
// guarenteed to no longer call that function after it first returns
// 'boost::none' indicating the end of the partial time definition.
//
// 'IncreasingPartialTimeFunction's 'pull()' function adds a few features to
// the underlying time function. First, it can be called with increasing values
// of time even after 'boost::none' has been reached. Second, the first time
// 'boost::none' is read from the underlying function, all that underlying
// function's resources are freed up. Finally, the 'pull()' function can be
// called repeatedly with the same time value.
//
// This last feature allows several differen nodes in a functional
// reactive programming graph to call the 'pull()' function of a single time
// function at a single time step.
//
// Generally speaking, this class is intended for use as a tool to build up
// an implementation of the sfrp_behavior component.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Because the effect of this class is only a slight enhancement upon
// sfrp_increasingpartialtimefunction, an example is omittied.

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <sfrp/cachedpull.hpp>
#include <sfrp/increasingpartialtimefunction.hpp>

namespace sfrp {

// This class implements a partial time function whose pull method allows for
// multiple subsequent calls of the same value.
template <typename Value>
struct CachedIncreasingPartialTimeFunction {

  // Create a new 'CachedIncreasingPartialTimeFunction<Value>' object from the
  // specified 'valuePullFunc' function.  The behavior is undefined unless
  // 'valuePullFunc' returns a value when it is called once and is defined with
  // increasing argument values as long as the previous call didn't return
  // 'boost::none'.
  CachedIncreasingPartialTimeFunction(
      boost::function<boost::optional<Value>(double)> valuePullFunc);

  // Create a new 'CachedIncreasingPartialTimeFunction' object that has the
  // same value as the specified 'other' object.
  CachedIncreasingPartialTimeFunction(
      CachedIncreasingPartialTimeFunction&& other);

  // Return the value of this partial time function at the specified 'time' if
  // it is defined, otherwise return 'boost::none'. If a previous call to
  // 'pull()' returns 'boost::none', so will all subsequent calls. The first
  // time 'boost::none' is returned, the underlying function object is garbage
  // collected.
  //
  // The behavior is undefined unless 'pull()' is being called for the first
  // time or the previous call to 'pull()' used a time argument that is less
  // than or equal to 'time'.
  //
  // Note that the aformentioned preconditions imply that 'pull()' is called
  // with increasing (or the same) time arguments and that after 'pull()'
  // returns 'boost::none', it will return 'boost::none' thereafter.
  boost::optional<Value> pull(const double time);

  // Give this 'CachedIncreasingPartialTimeFunction' object the same value as
  // the specified 'other' object.
  CachedIncreasingPartialTimeFunction& operator=(
      CachedIncreasingPartialTimeFunction&& other);

 private:
  // not implemented
  CachedIncreasingPartialTimeFunction(
      const CachedIncreasingPartialTimeFunction&);
  CachedIncreasingPartialTimeFunction& operator=(
      const CachedIncreasingPartialTimeFunction&);

  IncreasingPartialTimeFunction<Value> m_increasingPartialTimeFunction;
  boost::optional<CachedPull<Value>> m_previousPullCache;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Value>
CachedIncreasingPartialTimeFunction<Value>::CachedIncreasingPartialTimeFunction(
    boost::function<boost::optional<Value>(double)> valuePullFunc)
    : m_increasingPartialTimeFunction(valuePullFunc), m_previousPullCache() {}

template <typename Value>
CachedIncreasingPartialTimeFunction<Value>::CachedIncreasingPartialTimeFunction(
    CachedIncreasingPartialTimeFunction&& other) {
  m_increasingPartialTimeFunction =
      std::move(other.m_increasingPartialTimeFunction);
}

template <typename Value>
boost::optional<Value> CachedIncreasingPartialTimeFunction<Value>::pull(
    const double time) {
  if (m_previousPullCache && m_previousPullCache->time() == time) {
    return m_previousPullCache->value();
  } else {
    boost::optional<Value> result = m_increasingPartialTimeFunction.pull(time);
    if (!result)
      m_previousPullCache = boost::none;
    else
      m_previousPullCache = CachedPull<Value>(time, *result);
    return result;
  }
}
template <typename Value>
CachedIncreasingPartialTimeFunction<Value>&
CachedIncreasingPartialTimeFunction<Value>::
operator=(CachedIncreasingPartialTimeFunction&& other) {
  m_increasingPartialTimeFunction =
      std::move(other.m_increasingPartialTimeFunction);
}
}
#endif

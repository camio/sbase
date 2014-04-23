#ifndef SFRP_BEHAVIOR_HPP_
#define SFRP_BEHAVIOR_HPP_

#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/cachedincreasingpartialtimefunction.hpp>

namespace sfrp {

// This class implements a partial function from time to the specified 'Value'
// type. The function is defined from time '0' to some end time greater than
// '0'. Behaviors that are not defined at time '0', or any subsequent time, are
// permitted.
template <typename Value>
struct Behavior {

  // The codomain of this partial time function.
  typedef Value type;

  // Create a 'Behavior' object that is not defined at any time.
  Behavior();

  // Create a 'Behavior' object equivelent to the specified 'behavior'.
  Behavior(Behavior<Value>&& behavior);

  // Return the value of this partial time function at the specified 'time' if
  // it is defined, otherwise return 'boost::none'. If a previous call to
  // 'pull()' returns 'boost::none', so will all subsequent calls.  The
  // behavior is undefined unless 'pull()' is being called for the first time
  // or the previous call to 'pull()' used a time argument that is less than
  // 'time'. The behavior is also undefined unless 'time >= 0.0'.
  //
  // Note that the aformentioned preconditions imply that 'pull()' is called
  // with increasing time arguments and that after 'pull()' returns
  // 'boost::none', it will return 'boost::none' thereafter.
  boost::optional<Value> pull(const double time) const;

  // Create a new 'Behavior<Value>' object from the 'valuePullFunc' function.
  // The behavior is undefined unless 'valuePullFunc' returns a value when it
  // is called once and is defined with increasing argument values as long as
  // the previous call didn't return 'boost::none'.
  static Behavior<Value> fromValuePullFunc(
      boost::function<boost::optional<Value>(double)> valuePullFunc);

 private:
  mutable boost::shared_ptr<CachedIncreasingPartialTimeFunction<Value>> m_timeFunction;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A>
Behavior<A> Behavior<A>::fromValuePullFunc(
    boost::function<boost::optional<A>(double)> valuePullFunc) {
  Behavior<A> result;
  result.m_timeFunction =
      boost::make_shared<CachedIncreasingPartialTimeFunction<A>>(std::move(valuePullFunc));
  return result;
}

template <typename A>
Behavior<A>::Behavior()
    : m_timeFunction() {}

template <typename A>
boost::optional<A> Behavior<A>::pull(const double time) const {
  if (m_timeFunction) {
    boost::optional<A> result = m_timeFunction->pull(time);
    if (!result)
      m_timeFunction.reset();
    return result;
  } else
    return boost::none;
}

template <typename A>
Behavior<A>::Behavior(Behavior<A>&& behavior)
    : m_timeFunction(std::move(behavior.m_timeFunction)) {}
}
#endif

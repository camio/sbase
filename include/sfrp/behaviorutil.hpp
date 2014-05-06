#ifndef SFRP_BEHAVIORUTIL_HPP_
#define SFRP_BEHAVIORUTIL_HPP_

#include <sfrp/behavior.hpp>

namespace sfrp {
// This class implements a namespace for simple utility operations on
// 'Behavior' objects.
struct BehaviorUtil {

  // Return a behavior that has the specified 'value' for all time.
  template <typename T>
  static Behavior<T> always(const T& value);

  // Return a behavior that has the value of the current time at every moment.
  static Behavior< double > time( );

  // Return a behavior with values equivelent to the specified 'valueBehavior',
  // but is undefined whenever the specified 'curtailingBehavior' is undefined.
  template <typename T, typename U>
  static Behavior<T> curtail(const Behavior<T>& valueBehavior,
                             const Behavior<U>& curtailingBehavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Behavior<T> BehaviorUtil::always(const T& value) {
  return Behavior<T>::fromValuePullFunc([value](double time) {
    return boost::make_optional(value);
  });
}
template <typename T, typename U>
Behavior<T> BehaviorUtil::curtail(const Behavior<T>& valueBehavior,
                                  const Behavior<U>& curtailingBehavior)
{
  return Behavior<T>::fromValuePullFunc([valueBehavior, curtailingBehavior](
      double time) {
    boost::optional<T> value = valueBehavior.pull(time);
    boost::optional<U> curtail = curtailingBehavior.pull(time);
    return curtail ? value : boost::none;
  });
}
}
#endif

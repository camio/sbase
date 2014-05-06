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
}
#endif

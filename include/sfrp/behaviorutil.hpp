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
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
static Behavior<T> BehaviorUtil::always(const T& value) {
  return Behavior<T>::fromValuePullFunc([value](const double time) {
    return boost::make_optional(value);
  });
}
}
#endif

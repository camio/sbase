#ifndef SFRP_BEHAVIORDEBUGUTIL_HPP_
#define SFRP_BEHAVIORDEBUGUTIL_HPP_

#include <sfrp/behavior.hpp>

namespace sfrp {
struct BehaviorDebugUtil {
  // Return a behavior that is equivelent to the specified behavior with the
  // exception that the specified 'function' is called on the result of
  // 'behavior' as a side-effect of every pull.
  //
  //..
  //  watch : {T : Set} → IO T → Behavior T → Behavior T
  //..
  template <typename Function, typename T>
  Behavior<T> watch(Function function, const Behavior<T>& behavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename T>
Behavior<T> BehaviorDebugUtil::watch(Function function,
                                     const Behavior<T>& behavior) {
  return Behavior<T>::fromValuePullFunc([function, behavior](const double time)
                                            ->boost::optional<T> {
    T r = behavior.pull(time);
    if (r)
      function(*r);
    return r;
  });
}
}
#endif

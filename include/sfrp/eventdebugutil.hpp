#ifndef SFRP_EVENTDEBUGUTIL_HPP_
#define SFRP_EVENTDEBUGUTIL_HPP_

#include <boost/optional.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviordebugutil.hpp>

namespace sfrp {
struct EventDebugUtil {
  template <typename Function, typename T>
  Behavior<boost::optional<T>> watch(Function function,
                                     const Behavior<boost::optional<T>>& event);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename T>
Behavior<boost::optional<T>> EventDebugUtil::watch(
    Function function,
    const Behavior<boost::optional<T>>& event) {
  return BehaviorDebugUtil::watch([function](boost::optional<T> occurence) {
                                    if (occurance)
                                      f(*occurance);
                                  },
                                  event);
}
}
#endif

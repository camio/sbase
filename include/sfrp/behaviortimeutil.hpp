#ifndef SFRP_BEHAVIORTIMEUTIL_HPP_
#define SFRP_BEHAVIORTIMEUTIL_HPP_

#include <sfrp/behavior.hpp>
#include <sfrp/behaviorpairutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <utility>  // std::pair

namespace sfrp {
struct BehaviorTimeUtil {

  // Return a behavior equivelent to the specified 'behavior' except it is
  // paired up with the value of the previous pull to that behavior. The first
  // pull is paired up with the specified 'pull0' value.
  template <typename T>
  static Behavior<std::pair<T, T>> withPriorPull(const T& pull0,
                                                 const Behavior<T>& behavior);

  // Return a behavior equivelent to the specified 'behavior' except it is
  // paired up with the current time.
  template <typename T>
  static Behavior<std::pair<T, double>> withTime(const Behavior<T>& behavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Behavior<std::pair<T, T>> BehaviorTimeUtil::withPriorPull(
    const T& t0,
    const Behavior<T>& pm) {
  Wormhole<T> w = Wormhole<T>(t0);
  return sfrp::BehaviorPairUtil::makePair(w.outputBehavior(),
                                          w.setInputBehavior(pm));
}

template <typename T>
Behavior<std::pair<T, double>> BehaviorTimeUtil::withTime(
    const Behavior<T>& pm) {
  return sfrp::BehaviorPairUtil::makePair(pm, sfrp::BehaviorUtil::time());
}
}
#endif

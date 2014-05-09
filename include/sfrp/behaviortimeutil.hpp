#ifndef SFRP_BEHAVIORTIMEUTIL_HPP_
#define SFRP_BEHAVIORTIMEUTIL_HPP_

//@PURPOSE: Provide behavior utility operations related to time.
//
//@CLASSES:
//  sfrp::BehaviorTimeUtil: namespace for behavior time utility operations
//
//@DESCRIPTION: This component provides a single namespace class,
// 'BehaviorTimeUtil', which includes behavior utility operations related to
// time.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Derivative
// - - - - - - - - - - -
// With the two functions provided in this component, we can create a behavior
// derivative function. The key thing to note is that a piece-wise derivative is
// Δx/Δt. Each pull provides a different piece of the derivative.
//
// The argument to map below is the prior (value,time) pair paired with the
// current (value,time) pair.
//..
//  sfrp::Behavior<double> derivative(sfrp::Behavior<double> behavior) {
//    return sfrp::BehaviorUtil::map(
//        [](std::pair<std::pair<double, double>, std::pair<double, double>>
//               pairs) {
//          return (pairs.second.first - pairs.first.first) /
//                 (pairs.second.second - pairs.first.second);
//        },
//        sfrp::BehaviorTimeUtil::withPriorPull(
//            std::make_pair(0.0, 0.0),
//            sfrp::BehaviorTimeUtil::withTime(behavior)));
//  }
//..

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviorpairutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <utility>  // std::pair

namespace sfrp {
// This class implements a namespace for utility operations on
// 'Behavior' objects relating to time.
struct BehaviorTimeUtil {

  // Return a behavior equivelent to the specified 'behavior' except it is
  // paired up with the value of the previous pull to that behavior. The
  // "previous" value is the first element of the pair and the "current" value
  // is the second. The first pull is paired up with the specified 'pull0'
  // value.
  template <typename T>
  static Behavior<std::pair<T, T>> withPriorPull(const T& pull0,
                                                 const Behavior<T>& behavior);

  // Return a behavior equivelent to the specified 'behavior' except it is
  // paired up with the current time.
  template <typename T>
  static Behavior<std::pair<T, double>> withTime(const Behavior<T>& behavior);

  // Return a behavior equivelent to the specified 'behavior' except that is
  // value up until the first 'pull()' occurence is replaced with the specified
  // 'initialValue'.
  template <typename T>
  static Behavior<T> replaceInitialValue(const Behavior<T>& behavior,
                                         const T& initialValue);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Behavior<std::pair<T, T>> BehaviorTimeUtil::withPriorPull(
    const T& t0,
    const Behavior<T>& behavior) {
  Wormhole<T> wormhole = Wormhole<T>(t0);
  sfrp::Behavior<T> wormholeUpdateBehavior =
      wormhole.setInputBehavior(behavior);
  // This call is a bit subtle. We need to ensure that the pull on the pair
  // occurs before the pull on 'wormholeUpdateBehavior'. Otherwise,
  // 'wormhole.outputBehavior' may not be the delayed version.
  return sfrp::BehaviorUtil::curtail(
      sfrp::BehaviorPairUtil::makePair(wormhole.outputBehavior(), behavior),
      wormholeUpdateBehavior);
}

template <typename T>
Behavior<std::pair<T, double>> BehaviorTimeUtil::withTime(
    const Behavior<T>& pm) {
  return sfrp::BehaviorPairUtil::makePair(pm, sfrp::BehaviorUtil::time());
}

template <typename T>
static Behavior<T> BehaviorTimeUtil::replaceInitialValue(
    const Behavior<T>& behavior,
    const T& initialValue)
{
  boost::shared_ptr<bool> initialValueAlreadyPulled =
      boost::make_shared<bool>(false);
  return sfrp::Behavior<T>::fromValuePullFunc(
          [initialValueAlreadyPulled, behavior, initialValue](const double time)
              ->boost::optional<T> {
            if (*initialValueAlreadyPulled) {
              return behavior.pull(time);
            } else {
              behavior.pull(time);
              *initialValueAlreadyPulled = true;
              return boost::make_optional(initialValue);
            }
          });
}
}
#endif

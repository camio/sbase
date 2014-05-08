#ifndef SFRP_BEHAVIORPAIRUTIL_HPP_
#define SFRP_BEHAVIORPAIRUTIL_HPP_

//@PURPOSE: Provide utility operations for behaviors of 'std::pair's.
//
//@CLASSES:
//  sfrp::BehaviorPairUtil: behavior of pair utility operations
//
//@DESCRIPTION: This component provides a single namespace, 'BehaviorPairUtil',
// which include operations that provide for the creation and analysis of
// 'std::pair' behaviors.
//
// The functions provided here mimic those in the standard library '<utility>'
// header relating to pairs as well as pair member access operators.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Pair a string behavior with time
// - - - - - - - - - - - - - - - - - - - - - -
// Pairs are frequently used to combine multiple behaviors into a single
// behavior. In the following example we combine a 'sfrp::Behavior<std::string>'
// behavior with the time behavior.
//..
//  sfrp::Behavior<std::pair<std::string, double>> stringBehaviorWithTime =
//      sfrp::BehaviorPairUtil::makePair(stringBehavior,
//                                       sfrp::BehaviorUtil::time());
//..

#include <scpp/pairutil.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <utility>  // std::pair

namespace sfrp {

// This class implements a namespace for std::pair utility operations on
// 'Behavior' objects.
struct BehaviorPairUtil {

  // Return a behavior who's values are the pair of the values of the specified
  // 'first' and 'second' behaviors.
  template <typename A, typename B>
  static Behavior<std::pair<A, B>> makePair(const Behavior<A>& first,
                                            const Behavior<B>& second);

  // Return a behavior who's values are the first elements of the specified
  // 'pairBehavior'.
  template <typename A, typename B>
  static Behavior<A> first(const Behavior<std::pair<A, B>>& pairBehavior);

  // Return a behavior who's values are the second elements of the specified
  // 'pairBehavior'.
  template <typename A, typename B>
  static Behavior<B> second(const Behavior<std::pair<A, B>>& pairBehavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A, typename B>
Behavior<std::pair<A, B>> BehaviorPairUtil::makePair(const Behavior<A>& a,
                                                     const Behavior<B>& b) {
  return sfrp::BehaviorMap()(&scpp::PairUtil::makePair<A, B>, a, b);
}

template <typename A, typename B>
Behavior<A> BehaviorPairUtil::first(const Behavior<std::pair<A, B>>& pm) {
  return sfrp::BehaviorMap()(scpp::PairUtil::first<A, B>, pm);
}

template <typename A, typename B>
Behavior<B> BehaviorPairUtil::second(const Behavior<std::pair<A, B>>& pm) {
  return sfrp::BehaviorMap()(scpp::PairUtil::second<A, B>, pm);
}
}
#endif

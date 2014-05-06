#ifndef SFRP_BEHAVIORPAIRUTIL_HPP_
#define SFRP_BEHAVIORPAIRUTIL_HPP_

#include <scpp/pairutil.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <utility>  // std::pair

namespace sfrp {
struct BehaviorPairUtil {
  template <typename A, typename B>
  static Behavior<std::pair<A, B>> makePair(const Behavior<A>& a,
                                            const Behavior<B>& b);

  template <typename A, typename B>
  static Behavior<A> first(const Behavior<std::pair<A, B>>& pm);

  template <typename A, typename B>
  static Behavior<B> second(const Behavior<std::pair<A, B>>& pm);
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

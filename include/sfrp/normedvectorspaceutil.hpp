#ifndef SFRP_NORMEDVECTORSPACEUTIL_HPP_
#define SFRP_NORMEDVECTORSPACEUTIL_HPP_

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/behaviorpairutil.hpp>
#include <sfrp/behaviortimeutil.hpp>
#include <sfrp/wormhole.hpp>
#include <smisc/point1d.hpp>
#include <smisc/normedvectorspace.hpp>

namespace sfrp {

// This class is a namespace for frp functions that deal with normed vector
// spaces.
struct NormedVectorSpaceUtil {

  // Return a smoothed version of the specified 'behavior' allowing for a
  // maximum specified 'velocity'.
  template <typename V>
  static Behavior<V> smooth(const Behavior<smisc::Point1D> velocity,
                            const Behavior<V> behavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename V>
V VectorSpaceUtil_smoothHelper(const smisc::Point1D& ss,
                               const std::pair<V, double>& prev,
                               const std::pair<V, double>& cur) {
  // In this degenerate case, we just return
  // v. If we instead returned 0.0, we would be
  // discarding v's value which is lame. We may
  // want to revisit this choice at some point.
  if (cur.second == 0.0)
    return cur.first;

  const double dt = cur.second - prev.second;
  assert(dt > 0.0);
  assert(ss > 0.0);
  const V dv = cur.first - prev.first;
  const smisc::Point1D normDv = smisc::norm(dv);

  assert(normDv >= 0.0);
  // maxNormDv is always non-negative.
  const smisc::Point1D maxNormDv = ss * dt;
  assert(maxNormDv > 0.0);

  if (normDv <= maxNormDv) {
    return cur.first;
  } else {
    // normDv must not be 0 otherwise
    // we would be in the if case.
    const V dvDir = dv / normDv;
    const V dv2 = dvDir * maxNormDv;
    return prev.first + dv2;
  }
}

template <typename V>
Behavior<V> NormedVectorSpaceUtil::smooth(const Behavior<smisc::Point1D> v,
                                          const Behavior<V> behavior) {
  auto wh = sfrp::Wormhole<std::pair<V, smisc::Point1D>>(
      std::make_pair(smisc::zero<V>(), 0.0));

  return sfrp::BehaviorPairUtil::first(
      wh.setInputBehavior(sfrp::BehaviorTimeUtil::withTime(
          sfrp::BehaviorMap()(&VectorSpaceUtil_smoothHelper<V>,
                              v,
                              wh.outputBehavior(),
                              sfrp::BehaviorTimeUtil::withTime(behavior)))));
}
}

#endif

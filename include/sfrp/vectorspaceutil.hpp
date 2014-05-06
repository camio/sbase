#ifndef SFRP_VECTORSPACEUTIL_HPP_
#define SFRP_VECTORSPACEUTIL_HPP_

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/util.hpp>
#include <sfrp/wormhole.hpp>
#include <smisc/point1d.hpp>
#include <smisc/vectorspace.hpp>

namespace sfrp {

// This class is a namespace for frp functions that deal with vector spaces.
struct VectorSpaceUtil {

  // Return a behavior that is, at each pull, the sum of a pull to the specified
  // 'behavior' with all previous pulls to it. The result is undefined unless
  // 'T' is a 'smisc::VectorSpace'.
  template <typename T>
  static Behavior<T> sum(const Behavior<T>& behavior);

  // Return the piece-wise integral of the specified 'behavior'.  Say
  // 'Behavior::pull' is called on the result of this function, 'r', 'n'
  // different times. Lets call these times 'ts'. 'r' is defined to be the
  // piece-wise integral of 'v', broken up at 'ts.  The result is undefined
  // unless 'T' is a 'smisc::VectorSpace'.
  template <typename T>
  static Behavior<T> integral(const Behavior<T>& v);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
T VectorSpaceUtil_Sum(const T& a, const T& b) {
  return a + b;
}
;
template <typename T>
Behavior<T> VectorSpaceUtil::sum(const Behavior<T>& v) {
  auto wh = sfrp::Wormhole<T>(smisc::zero<T>());
  return wh.setInputBehavior(
      sfrp::BehaviorMap()(&VectorSpaceUtil_Sum<T>, wh.outputBehavior(), v));
}

template <typename V>
struct VectorSpaceUtil_Integral {
  typedef V result_type;

  V operator()(
      const std::pair<std::pair<V, double>, std::pair<V, double>> vs) const {
    const double time = vs.second.second;
    const double timePrev = vs.first.second;
    const V v = vs.second.first;
    // const V vPrev = vs.first.first;
    // TODO: do something better here using vPrev
    return (time - timePrev) * v;
  }
};

template <typename T>
Behavior<T> VectorSpaceUtil::integral(const Behavior<T>& v) {
  const Behavior<T> slices = sfrp::BehaviorMap()(
      VectorSpaceUtil_Integral<T>(),
      pmWithPrev(std::make_pair(smisc::zero<T>(), 0.0), pmWithTime(v)));
  return VectorSpaceUtil::sum(slices);
}
}
#endif

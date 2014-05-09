#ifndef SFRP_VECTORSPACEUTIL_HPP_
#define SFRP_VECTORSPACEUTIL_HPP_

//@PURPOSE: Provide utility operations on vector space behaviors.
//
//@CLASSES:
//  sfrp::VectorSpaceUtil: vector space behavior operations namespace
//
//@SEE_ALSO: sfrp_normedvectorspaceutil, smisc_vectorspace
//
//@DESCRIPTION: This component provides one namespace class, 'VectorSpaceUtil',
// which provides integration and other vector space functionality.
//
// The 'integral()' function can be used to calculate the position of objects
// based on preset velocities and accelerations. This is useful for gravity
// simulation and other momentum operations.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Position Calculation
// - - - - - - - - - - - -- - - -
// Calculating the position of an object given an initial position, a velocity,
// and an acceleration is a straightforward application of the physics
// equations.
//..
//  static sfrp::Behavior<double> calculatePosition(
//      double initialPosition,
//      sfrp::Behavior<double> velocity,
//      sfrp::Behavior<double> acceleration) {
//    return sfrp::BehaviorUtil::always(initialPosition) +
//           sfrp::VectorSpaceUtil::integral(
//               velocity + sfrp::VectorSpaceUtil::integral(acceleration));
//  }
//..

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/behaviortimeutil.hpp>
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
Behavior<T> VectorSpaceUtil::sum(const Behavior<T>& v) {
  auto wh = sfrp::Wormhole<T>(smisc::zero<T>());
  return wh.setInputBehavior(
      sfrp::BehaviorMap()([](const T & a, const T & b) { return a + b; },
                          wh.outputBehavior(),
                          v));
}

template <typename T>
Behavior<T> VectorSpaceUtil::integral(const Behavior<T>& v) {
  const Behavior<T> slices = sfrp::BehaviorMap()(
      // Calculate the slice by computing the volume of the rectangle specified
      // by the current value, the current time, and the previous time. At some
      // point it may be interesting to make this more sophisiticated by using
      // the previous value and computing the volume of a trapezoid instead of a
      // square.
          [](const std::pair<std::pair<T, double>, std::pair<T, double>> vs)
              ->T const {
            const double time = vs.second.second;
            const double timePrev = vs.first.second;
            const T v = vs.second.first;
            return (time - timePrev) * v;
          },
          sfrp::BehaviorTimeUtil::withPriorPull(
              std::make_pair(smisc::zero<T>(), 0.0),
              sfrp::BehaviorTimeUtil::withTime(v)));
  return VectorSpaceUtil::sum(slices);
}
}
#endif

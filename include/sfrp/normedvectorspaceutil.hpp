#ifndef SFRP_NORMEDVECTORSPACEUTIL_HPP_
#define SFRP_NORMEDVECTORSPACEUTIL_HPP_

//@PURPOSE: Provide utility operations on normed vector space behaviors.
//
//@CLASSES:
//  sfrp::NormedVectorSpaceUtil: normed vector space behavior namespace
//
//@SEE_ALSO: sfrp_vectorspaceutil, smisc_normedvectorspace
//
//@DESCRIPTION: This component provides one namespace class,
// 'NormedVectorSpaceUtil', which has a single function that smooths behaviors.
// The 'smooth()' function takes in a target behavior and a velocity behavior
// and returns a behavior that attempts to follow the targe behavior with the
// constraint that its speed cannot exceed the velocity behavior.
//
// This function is intended for use ranging from moving to "teach point"
// positions in robotics to making smooth looking animations in artistry work.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Heat seaking missile
// - - - - - - - - - - - -- - - -
// Imagine that in a game, a heat seeking missile is emitted that follows a
// particular enemy ship. This can be implemented using the 'smooth()' function.
// 

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

  // Return a smoothed version of the specified 'target' behavior allowing for
  // a maximum specified 'maxVelocity'. The initial pull of the result of this
  // function will always be equivelent to the initial pull of 'target'.
  // Thereafter, the resulting behavior attempts to smoothly match 'target'.
  // The behavior is undefined unless 'maxVelocity' is greater than '0' for all
  // time values.
  //
  // Note that the resulting behavior will always try to reach 'target', but
  // will never have a velocity higher than 'maxVelocity'.
  template <typename NormedVectorSpace>
  static Behavior<NormedVectorSpace> smooth(
      const Behavior<smisc::Point1D> maxVelocity,
      const Behavior<NormedVectorSpace> target);

 private:
  // Return the value closest to the specified 'targetValueWithTime's
  // value moving from 'currentValueWithTime's value without exceeding the
  // specified 'velocity'. If 'currentValueWithTime.second' is '-1.0', then
  // always return 'targetValueWithTime.first'. The behavior is undefined
  // unless 'maxVelocity > 0.0'.
  template <typename NormedVectorSpace>
  static NormedVectorSpace smoothHelper(
      const smisc::Point1D& maxVelocity,
      const std::pair<NormedVectorSpace, double>& currentValueWithTime,
      const std::pair<NormedVectorSpace, double>& targetValueWithTime);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename NormedVectorSpace>
NormedVectorSpace NormedVectorSpaceUtil::smoothHelper(
    const smisc::Point1D& maxVelocity,
    const std::pair<NormedVectorSpace, double>& currentValueWithTime,
    const std::pair<NormedVectorSpace, double>& targetValueWithTime) {

  // When this is the first 'pull', we accept the target position as the
  // starting position.
  if (currentValueWithTime.second == -1.0)
    return targetValueWithTime.first;

  const double dt = targetValueWithTime.second - currentValueWithTime.second;
  assert(dt > 0.0);
  assert(maxVelocity > 0.0);
  const NormedVectorSpace dv =
      targetValueWithTime.first - currentValueWithTime.first;
  const smisc::Point1D normDv = smisc::norm(dv);

  assert(normDv >= 0.0);
  // maxNormDv is always non-negative.
  const smisc::Point1D maxNormDv = maxVelocity * dt;
  assert(maxNormDv > 0.0);

  if (normDv <= maxNormDv) {
    return targetValueWithTime.first;
  } else {
    // normDv must not be 0 otherwise
    // we would be in the if case.
    const NormedVectorSpace dvDir = dv / normDv;
    const NormedVectorSpace dv2 = dvDir * maxNormDv;
    return currentValueWithTime.first + dv2;
  }
}

template <typename NormedVectorSpace>
Behavior<NormedVectorSpace> NormedVectorSpaceUtil::smooth(
    const Behavior<smisc::Point1D> maxVelocity,
    const Behavior<NormedVectorSpace> target) {

  // We're using '-1.0' as a sentinel value to indicate that this value has no
  // basis in reality.
  auto wh = sfrp::Wormhole<std::pair<NormedVectorSpace, smisc::Point1D>>(
      std::make_pair(smisc::zero<NormedVectorSpace>(), -1.0));

  return sfrp::BehaviorPairUtil::first(
      wh.setInputBehavior(sfrp::BehaviorTimeUtil::withTime(sfrp::BehaviorMap()(
          &NormedVectorSpaceUtil::smoothHelper<NormedVectorSpace>,
          maxVelocity,
          wh.outputBehavior(),
          sfrp::BehaviorTimeUtil::withTime(target)))));
}
}

#endif

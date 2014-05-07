#ifndef SFRP_BEHAVIORUTIL_HPP_
#define SFRP_BEHAVIORUTIL_HPP_

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <type_traits>

namespace sfrp {
// This class implements a namespace for simple utility operations on
// 'Behavior' objects.
struct BehaviorUtil {

  // Return a behavior that has the specified 'value' for all time.
  template <typename T>
  static Behavior<T> always(const T& value);

  // Return a behavior that has the value of the current time at every moment.
  static Behavior<double> time();

  // Return a behavior that, at any time 't', has a value of the specified
  // 'timeFunction' applied to 't'. The behavior is undefined unless
  // 'timeFunction' is defined for all input values.
  template <typename Function>
  static Behavior<typename std::result_of<Function(double)>::type> pure(
      Function timeFunction);

  // Return a behavior with values equivelent to the specified 'valueBehavior',
  // but is undefined whenever the specified 'curtailingBehavior' is undefined.
  // The 'valueBehavior' is always pulled prior to the 'curtailingBehavior'.
  //
  // Note that 'curtial()' can be used to ensure that behaviors are pulled even
  // if they aren't used. This application is frequently useful with wormholes,
  // for example.
  template <typename T, typename U>
  static Behavior<T> curtail(const Behavior<T>& valueBehavior,
                             const Behavior<U>& curtailingBehavior);

  // Return a behavior that, for any time 't', is defined to be the specified
  // 'function' applied to the specified 'argBehaviors' behaviors at time 't'.
  // If any of 'argBehaviors' is not defined at time 't', then neither is the
  // result behavior.
  //
  // The behavior is undefined unless the specified 'Function' meets the
  // 'Deferred Callable Object' concept of Boost.Fusion.
  template <typename Function, typename... ArgBehaviors>
  static typename std::result_of<BehaviorMap(Function, ArgBehaviors...)>::type
      map(Function function, ArgBehaviors... argBehaviors);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Behavior<T> BehaviorUtil::always(const T& value) {
  return Behavior<T>::fromValuePullFunc([value](double time) {
    return boost::make_optional(value);
  });
}

template <typename T, typename U>
Behavior<T> BehaviorUtil::curtail(const Behavior<T>& valueBehavior,
                                  const Behavior<U>& curtailingBehavior) {
  return Behavior<T>::fromValuePullFunc([valueBehavior, curtailingBehavior](
      double time) {
    boost::optional<T> value = valueBehavior.pull(time);
    boost::optional<U> curtail = curtailingBehavior.pull(time);
    return curtail ? value : boost::none;
  });
}

template <typename Function>
static Behavior<typename std::result_of<Function(double)>::type>
BehaviorUtil::pure(Function timeFunction) {
  typedef typename std::result_of<Function(double)>::type ResultBehaviorValue;
  return Behavior<ResultBehaviorValue>::fromValuePullFunc([timeFunction](
      double time) { return boost::make_optional(timeFunction(time)); });
}

template <typename Function, typename... ArgBehaviors>
typename std::result_of<BehaviorMap(Function, ArgBehaviors...)>::type
BehaviorUtil::map(Function function, ArgBehaviors... argBehaviors) {
  return sfrp::BehaviorMap()(function, argBehaviors...);
}
}
#endif

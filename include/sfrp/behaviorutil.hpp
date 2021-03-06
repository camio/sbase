#ifndef SFRP_BEHAVIORUTIL_HPP_
#define SFRP_BEHAVIORUTIL_HPP_

//@PURPOSE: Provide utility operations for 'Behavior' objects.
//
//@CLASSES:
//  sfrp::BehaviorUtil: namespace for behavior utility operations.
//
//@SEE_ALSO: sfrp_behaviormap
//
//@DESCRIPTION: This component provides a single namespace class,
// 'BehaviorUtil', which has several utility functions operating on 'Behavior'
// objects. The operations provided here are "primitive" in the sense that they
// provide a purely functional interface to 'Behavior's as opposed to the
// built-in 'fromValuePullFunc()' function. These are the core building blocks
// for more complex behaviors.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Behavior Creation
// - - - - - - - - - - - - - -
// Perhaps the simplest behavior is one that always has the same value at every
// time. The following behavior is always '3'.
//..
//  sfrp::Behavior<int> always3 = sfrp::BehaviorUtil::always(3);
//..
// Another common behavior is the behavior that has a value equivelent to the
// time for every time. For example, at time '0.0', it will have the value
// '0.0'. At time '5.0', it will have value '5.0' and so on.
//..
//  sfrp::Behavior<double> time = sfrp::BehaviorUtil::time();
//..
// The 'pure' function directly converts a function of time into a behavior.
// Here we have a behavior that has a value of the time multiplied by '2'.
//..
//  sfrp::Behavior<double> timeTimesTwo =
//      sfrp::BehaviorUtil::pure([](double time) { return time * 2; });
//..
// Another example of 'pure' is a behavior that has value "hello" before time
// '3' and value "world" afterwards.
//..
//  sfrp::Behavior<std::string> switchAtTime3 = sfrp::BehaviorUtil::pure([](
//      double time) { return time < 3 ? std::string("hello") : "world"; });
//..
//
// Example 2: Curtail
// - - - - - - - - -
// A FRP execution ends when a behavior is no longer defined. Lets say we have
// an animation that loops forever called 'loopingAnimation' of type
// 'sfrp::Behavior<Drawing>'. If we'd like to stop it after 500 seconds, we
// first construct a behavior that is undefined after time '500.0'.
//..
//  sfrp::Behavior<smisc::Unit> undefinedAfter500 =
//      sfrp::Behavior<smisc::Unit>::fromValuePullFunc([](double time) {
//        return time < 500.0 ? boost::make_optional(smisc::Unit()) : boost::none;
//      });
//..
// Its value, 'smisc::Unit', carries no information so it could be any type
// really. We now use curtial to make our looping behavior stop after 500
// seconds.
//..
//  sfrp::Behavior<Drawing> loopingAnimationUntil500 =
//      sfrp::BehaviorUtil::curtail(loopingAnimation, undefinedAfter500);
//..
// Curtail ensures that the resulting behavior is only defined when both of its
// input behaviors are defined.
//
// The fact that 'curtail()' needs to always check its second argument for
// definedness gives it another use as well. When there is a behavior whose
// value isn't needed, but still needs to be pulled regularly for some
// side-effect, 'curtail()' can find use as well.
//
// Example 3: Math Functions
// - - - - - - - - - - - - -
// The 'map()' function takes functions over values and lifts them to functions
// over behaviors. For instance, lets say we wish to create a 'sin' function
// that works on behaviors.
//..
//  sfrp::Behavior<double> sin( sfrp::Behavior<double> value) {
//    return sfrp::BehaviorUtil::map([](double d) { return std::sin(d); }, value);
//  }
//..
// In this case we wrap 'std::sin' into an anonymous function because 'std::sin'
// is an overloaded function and we need to disambiguate somehow.
//
// 'map()' can be used with any number of arguments. This is how we would wrap
// the 'std::atan2' function:
//..
//  sfrp::Behavior<double> atan2(sfrp::Behavior<double> y,
//                               sfrp::Behavior<double> x) {
//    return sfrp::BehaviorUtil::map([](double y,
//                                      double x) { return std::atan2(y, x); },
//                                   y,
//                                   x);
//  }
//..

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

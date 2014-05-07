#ifndef SFRP_BEHAVIORDEBUGUTIL_HPP_
#define SFRP_BEHAVIORDEBUGUTIL_HPP_

//@PURPOSE: Provide functions that assist in the debugging of behaviors.
//
//@CLASSES:
//  sfrp::BehaviorDebugUtil: behavior debug utility namespace
//
//@SEE_ALSO: sfrp_eventdebugutil
//
//@DESCRIPTION: This component provides a single class, 'BehaviorDebugUtil',
// that serves as a namespace for behavior debugging functions.
//
// The 'watch()' function provides a means to call a function whenever a
// particular behavior is pulled. This is useful for logging or display of
// behaviors deep within a FRP graph. It is the analogue of using 'printf'
// statements for debugging imperative C++ programs.
//
// Although intended purely as a debugging tool, 'watch()' can also be used to
// perform other imperative side-effects in hybrid FRP/imperative designs.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Monitor a string behavior
// - - - - - - - - - - - - - - - - - -
// Assume we have a behavior of type 'sfrp::Behavior<std::string>' called
// 'behaviorString'. If we wish to keep track of its values, we can use the
// 'watch()' function to output its values to the console as they are read.
//..
//  sfrp::Behavior<std::string> watchedString =
//      sfrp::BehaviorDebugUtil::watch([](std::string s) {
//                                       std::cout << "Received " << s
//                                                 << std::endl;
//                                     },
//                                     behaviorString);
//..
// Note that the 'watchedString' behavior must now be used in place of where the
// original behavior was used to ensure that console logging side effect will be
// used.
//
// Although the above example used 'std::cout' as a means to visualize the value
// of the behavior, one could alternatively use a real time rendering mechanism
// to do the same.

#include <boost/optional.hpp>
#include <sfrp/behavior.hpp>

namespace sfrp {

// This class provides a namespace for utility operations on 'Behavior' objects
// related to debugging.
struct BehaviorDebugUtil {
  // Return a behavior that is equivelent to the specified 'behavior' with the
  // exception that the specified 'function' is called on the result of
  // 'behavior' as a side-effect of every pull.
  //
  //..
  //  watch : {T : Set} → IO T → Behavior T → Behavior T
  //..
  template <typename Function, typename T>
  static Behavior<T> watch(Function function, const Behavior<T>& behavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename T>
Behavior<T> BehaviorDebugUtil::watch(Function function,
                                     const Behavior<T>& behavior) {
  return Behavior<T>::fromValuePullFunc([function, behavior](const double time)
                                            ->boost::optional<T> {
    boost::optional<T> r = behavior.pull(time);
    if (r)
      function(*r);
    return r;
  });
}
}
#endif

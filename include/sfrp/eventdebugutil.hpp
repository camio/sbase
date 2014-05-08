#ifndef SFRP_EVENTDEBUGUTIL_HPP_
#define SFRP_EVENTDEBUGUTIL_HPP_

//@PURPOSE: Provide functions for the debugging of event-like behaviors.
//
//@CLASSES:
//  sfrp::EventDebugUtil: event debug utility namespace
//
//@SEE_ALSO: sfrp_behaviordebugutil
//
//@DESCRIPTION: This component provides a single class, 'EventDebugUtil',
// that serves as a namespace for event debugging functions.
//
// The 'watch()' function provides a means to call a function whenever a
// particular event occurs. This is useful for logging or display of
// events deep within a FRP graph. It is the analogue of using 'printf'
// statements for debugging imperative C++ programs.
//
// Although intended purely as a debugging tool, 'watch()' can also be used to
// perform other imperative side-effects in hybrid FRP/imperative designs.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Monitor a string event
// - - - - - - - - - - - - - - - - - -
// Assume we have an event of type
// 'sfrp::Behavior<boost::optional<std::string>>' called 'stringEvent'. If we
// wish to keep track of its values, we can use the 'watch()' function to
// output its values to the console as they are read.
//..
//  sfrp::Behavior<boost::optional<std::string>> watchedEvent =
//      sfrp::EventDebugUtil::watch([](std::string s) {
//                                    std::cout << "Received " << s << std::endl;
//                                  },
//                                  stringEvent);
//..
// Note that the 'watchedEvent' behavior must now be used in place of where the
// original behavior was used to ensure that console logging side effect will
// happen.
//
// Although the above example used 'std::cout' as a means to visualize the value
// of the behavior, one could alternatively use a real time rendering mechanism
// to do the same.

#include <boost/optional.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviordebugutil.hpp>

namespace sfrp {
// This class provides a namespace for utility operations on event-like
// 'Behavior' objects related to debugging.
struct EventDebugUtil {
  // Return a event-like behavior that is equivelent to the specified 'event' with the
  // exception that the specified 'function' is called on the result of
  // 'behavior' as a side-effect of every not-none pull.
  template <typename Function, typename T>
  static Behavior<boost::optional<T>> watch(Function function,
                                     const Behavior<boost::optional<T>>& event);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename T>
Behavior<boost::optional<T>> EventDebugUtil::watch(
    Function function,
    const Behavior<boost::optional<T>>& event) {
  return BehaviorDebugUtil::watch([function](boost::optional<T> occurrence) {
                                    if (occurrence)
                                      function(*occurrence);
                                  },
                                  event);
}
}
#endif

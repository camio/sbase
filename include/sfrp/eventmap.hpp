#ifndef SFRP_EVENTMAP_HPP_
#define SFRP_EVENTMAP_HPP_

#include <sfrp/behaviormap.hpp>
#include <sfrp/eventmapfunctionadapter.hpp>
#include <type_traits>

namespace sfrp {
struct EventMap {
  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return an event that occurs whenever the second argument to this function
  // occurs. Occurances will be the specified 'function' applied to value of the
  // event argument along with the values of each of the behavior arguments.
  // If any of 'argBehaviors' is not defined at time 't', then neither is the
  // result behavior.
  //
  // The behavior is undefined unless the specified 'Function' meets the
  // 'Deferred Callable Object' concept of Boost.Fusion.
  template <typename Function, typename... ArgBehaviors>
  typename result<EventMap(Function, ArgBehaviors...)>::type operator()(
      Function function,
      ArgBehaviors... argBehaviors) const;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename... ArgBehaviors>
struct EventMap::result<EventMap(Function, ArgBehaviors...)> {
  typedef sfrp::EventMapFunctionAdapter<typename std::remove_reference<
      typename std::remove_const<Function>::type>::type> AdaptedFunction;
  typedef typename std::result_of<
      BehaviorMap(AdaptedFunction, ArgBehaviors...)>::type type;
};

template <typename Function, typename... ArgBehaviors>
typename EventMap::result<EventMap(Function, ArgBehaviors...)>::type EventMap::
operator()(Function function, ArgBehaviors... argBehaviors) const {
  return BehaviorMap()(sfrp::EventMapFunctionAdapter<Function>(function),
                       argBehaviors...);
}
}
#endif

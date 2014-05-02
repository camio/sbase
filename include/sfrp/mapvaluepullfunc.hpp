#ifndef SFRP_MAPVALUEPULLFUNC_HPP_
#define SFRP_MAPVALUEPULLFUNC_HPP_

//@PURPOSE: Provide functor that pulls behaviors and applies a function to them.
//
//@CLASSES:
//  sfrp::MapValuePullFunc_ArgumentStorage: behavior storage metafunction
//  sfrp::MapValuePullFunc_Result: MapValuePullFunc result type metafunction
//  sfrp::MapValuePullFunc: behavior function application functor
//
//@SEE_ALSO: sfrp_behaviormap
//
//@DESCRIPTION: This component includes several classes that relate to
// implementation of a functor ('MapValuePullFunc') that, when executed, pulls
// several behaviors and applies a function to those results. The intent is for
// these classes to be used as helper functions in a behavior mapping
// implementation.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Pulling a function over a behavior
// - - - - - - - - - - - - - - - - - - - - - - -
//
// In this example we show how to create and use a 'MapValuePullFunc' object
// that, when called, applies a function to a behavior at a specified time.
//
// First we create our behavior and our function.
//..
//  // Set 'always3Behavior' to a behavior that always returns 3.
//  sfrp::Behavior<int> always3Behavior =
// sfrp::Behavior<int>::fromValuePullFunc([](
//      double time) { return boost::make_optional(3); });
//
//  // Create a 'toStringFunction' that converts 'int's into 'string's.
//  auto toStringFunction = [](int i) { return std::to_string(i); };
//  typedef decltype(toStringFunction) ToStringFunction;
//..
// Note that we extracted the type of the function into the 'ToStringFunction'
// typedef. This is required as a template argument to 'MapValuePullFunc'. The
// constructor to 'MapValuePullFunc' takes in the function and on behavior for
// each argument, in this case one.
//..
//  // Initialize our 'mapValuePullFunc' object with our function and behavior.
//  sfrp::MapValuePullFunc<ToStringFunction, sfrp::Behavior<int>>
// mapValuePullFunc(
//      toStringFunction, always3Behavior);
//..
// Finally, one calls 'mapValuePullFunc' with the times that values need to be
// extracted from. The result of the pull is the function applied to the pull of
// the underlying behavior. If the underlying behavior stops being defined, then
// 'mapValuePullFunc' will return 'boost::none'. In this case, the underlying
// behavior is always behind.
//..
//  // 'result' is set to 'optional<string>("3")'
//  boost::optional<std::string> result = mapValuePullFunc(0.0);
//..

#include <boost/fusion/include/all.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/invoke.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <sboost/optionalutil.hpp>  // OptionalUtil_HasValue
#include <sfrp/behavior.hpp>
#include <sfrp/behaviorpuller.hpp>
#include <type_traits>  // std::remove_const, std::remove_reference

namespace sfrp {

// This class implements a metafunction that returns an appropriate type for
// storing th especified 'ArgumentBehaviors'. The 'AagumentBehaviors' list may
// include 'const' and '&' qualifiers, but the resulting storage type will
// always be plain values.
//
// The behavior is undefined unless 'ArgumentBehaviors' consists of a list of
// possibly 'const' and/or '&' qualified Behaviors.
template <typename... ArgumentBehaviors>
struct MapValuePullFunc_ArgumentStorage {
  typedef boost::mpl::vector<ArgumentBehaviors...> ArgumentBehaviorsMplVector;
  typedef typename boost::mpl::transform<
      ArgumentBehaviorsMplVector,
      std::remove_const<std::remove_reference<boost::mpl::_1>>>::type
      ArgumentBehaviorsSanitized;
  typedef typename boost::fusion::result_of::as_vector<
      ArgumentBehaviorsSanitized>::type type;
};

// This class implements a metafunction that returns the result type of applying
// the specified 'Function' to a pull of the specified 'ArgumentBehaviors'.
// Because a pull of 'ArgumentBehaviors' results in an optional value, the
// result of this metafunction is also an optional value.
//
// The behavior is undefined unless 'Function' can take in the behavior values
// types of 'ArgumentBehaviors' as its arguments and return a result. The
// behavior is also undefined unless 'ArgumentBehaviors' consists of a list of
// possibly 'const' and/or '&' qualified Behaviors.  The behavior is also
// undefined unless the specified 'Function' meets the 'Deferred Callable
// Object' concept of Boost.Fusion.
template <typename Function, typename... ArgumentBehaviors>
struct MapValuePullFunc_Result {
  typedef typename MapValuePullFunc_ArgumentStorage<ArgumentBehaviors...>::type
      ArgumentStorage;
  typedef boost::optional<typename boost::fusion::result_of::invoke<
      Function,
      typename boost::fusion::result_of::transform<
          typename boost::fusion::result_of::transform<ArgumentStorage,
                                                       BehaviorPuller>::type,
          sboost::OptionalUtil_GetValue>::type>::type> type;
};

// This class implements a functor that, when called with a time argument, pulls
// a list of behaviors and applies a function to the results of those pulls.
template <typename Function, typename... ArgumentBehaviors>
struct MapValuePullFunc {
  // Create a new 'MapValuePullFunc' object with the specified 'function' and
  // 'argumentBehaviors'. The result is undefined unless the specified function
  // is defined with arguments coresponding to the behavior values implicit in
  // 'argumentBehaviors'. The behavior is also undefined unless
  // 'ArgumentBehaviors' consists of a list of possibly 'const' and/or '&'
  // qualified Behaviors.
  MapValuePullFunc(Function function, ArgumentBehaviors... argumentBehaviors);

  // The result type of the 'operator()' function.
  typedef typename MapValuePullFunc_Result<Function, ArgumentBehaviors...>::type
      result_type;

  // Pull all the argument behaviors at the specified 'time'. If they are all
  // not-none, return the result of applying the function to the results of
  // those pulls. Otherwise, return 'boost::none'.
  result_type operator()(const double time) const;

 private:
  Function m_function;
  typename MapValuePullFunc_ArgumentStorage<ArgumentBehaviors...>::type
      m_argumentBehaviors;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Function, typename... ArgumentBehaviors>
MapValuePullFunc<Function, ArgumentBehaviors...>::MapValuePullFunc(
    Function function,
    ArgumentBehaviors... argumentBehaviors)
    : m_function(function), m_argumentBehaviors(argumentBehaviors...) {}

template <typename Function, typename... ArgumentBehaviors>
typename MapValuePullFunc<Function, ArgumentBehaviors...>::result_type
MapValuePullFunc<Function, ArgumentBehaviors...>::
operator()(const double time) const {
  const auto pullResults = boost::fusion::as_vector(
      boost::fusion::transform(m_argumentBehaviors, BehaviorPuller(time)));

  const bool allPullsHaveValue =
      boost::fusion::all(pullResults, sboost::OptionalUtil_HasValue());

  if (allPullsHaveValue) {
    return boost::make_optional(boost::fusion::invoke(
        m_function,
        boost::fusion::transform(pullResults,
                                 sboost::OptionalUtil_GetValue())));
  } else {
    return boost::none;
  }
}
}
#endif

#ifndef SFRP_EVENTMAPFUNCTIONADAPTER_HPP_
#define SFRP_EVENTMAPFUNCTIONADAPTER_HPP_

#include <boost/optional.hpp>
#include <type_traits>

namespace sfrp {

template <typename F>
struct EventMapFunctionAdapter {
  EventMapFunctionAdapter(F f);

  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  template <typename OptionalArg, typename... ValueArgs>
  typename result<EventMapFunctionAdapter(OptionalArg, ValueArgs...)>::type
      operator()(OptionalArg optionalArg, ValueArgs... ValueArgs) const;

 private:
  F m_function;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template< typename Function>
template <typename OptionalArg, typename... ValueArgs>
struct EventMapFunctionAdapter<Function>::result<
    EventMapFunctionAdapter<Function>( OptionalArg, ValueArgs...)> {
  typedef typename std::remove_reference<
      typename std::remove_const<OptionalArg>::type>::type OptionalArgPlain;
  typedef typename OptionalArgPlain::value_type OptionalArgValue;

  typedef typename std::result_of<
      Function(OptionalArgValue, ValueArgs...)>::type FunctionResult;
  typedef boost::optional<FunctionResult> type;
};

template <typename Function>
EventMapFunctionAdapter<Function>::EventMapFunctionAdapter(Function f)
    : m_function(f) {}

template <typename Function>
template <typename OptionalArg, typename... ValueArgs>
typename EventMapFunctionAdapter<Function>::result<
    EventMapFunctionAdapter<Function>(OptionalArg, ValueArgs...)>::type
EventMapFunctionAdapter<Function>::
operator()(OptionalArg optionalArg, ValueArgs... valueArgs) const {
  if (optionalArg) {
    return m_function(*optionalArg, valueArgs...);
  } else {
    return boost::none;
  }
}
}
#endif

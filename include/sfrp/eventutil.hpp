#ifndef SFRP_EVENTUTIL_HPP_
#define SFRP_EVENTUTIL_HPP_

#include <boost/optional.hpp>
#include <sboost/optionalutil.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/behavioroperators.hpp>
#include <sfrp/behaviorutil.hpp>
#include <sfrp/eventmap.hpp>
#include <sfrp/wormhole.hpp>
#include <utility>  // std::make_pair

namespace sfrp {

// This class implements a namespace for simple utility operations on
// event-like 'Behavior' objects.
struct EventUtil {

  // Return a behavior that has an initial value of the specified 'initialValue'
  // which changes to the a new value at every occurence of the specified
  // 'event'.
  template <typename T>
  static Behavior<T> step(const T& t0,
                          const Behavior<boost::optional<T>>& event);

  // Return an event that never occurs.
  template <typename T>
  static Behavior<boost::optional<T>> never();

  // Return an event whose occurances are the union of the occurances of the
  // specified 'leftEvent' and 'rightEvent'. Preference is given to 'leftEvent'
  // if there is an occurance at the same time.
  template <typename A>
  static Behavior<boost::optional<A>> merge(
      const Behavior<boost::optional<A>>& leftEvent,
      const Behavior<boost::optional<A>>& rightEvent);

  // Return an event whose occurances are the union of the occurances of the
  // specified 'leftEvent' and 'rightEvent'. If both events occur at the same
  // time, the specified 'function' is used to collect the results.
  template <typename Function, typename A>
  static Behavior<boost::optional<A>> mergeWith(
      Function function,
      const Behavior<boost::optional<A>>& leftEvent,
      const Behavior<boost::optional<A>>& rightEvent);

  // Return an event whose occurances are those of the specified 'event' where
  // the specified 'function' returns 'true'.
  template <typename Function, typename A>
  static Behavior<boost::optional<A>> filter(
      Function function,
      const Behavior<boost::optional<A>>& event);

  // Return an event whose values are the result of repeated applications of the
  // function within occurances of the specified 'accumulator'. The specified
  // 'initialValue' is fed into the initial occurance of 'accumulator'. The
  // returned behavior's occurance times will be the same as those of
  // 'accumulator'.
  template <typename Function, typename A>
  static Behavior<boost::optional<A>> accumulate(
      A initialValue,
      const Behavior<boost::optional<Function>> accumulator );

  // Return an event with the same occurances of the specified 'event' except
  // they are paired with the values of the specified 'behavior' at those times.
  template <typename A, typename B>
  static Behavior<boost::optional<std::pair<A, B>>> snapshot(
      const Behavior<B>& behavior,
      const Behavior<boost::optional<A>>& event);

  // Return an event with the same occurances of the specified 'event' except
  // the values are those of the specified 'behavior' at those times.
  template <typename A, typename B>
  static Behavior<boost::optional<B>> snapshot_(
      const Behavior<B>& behavior,
      const Behavior<boost::optional<A>>& event);

  // Return an event with the same occurances of the specified 'event' except
  // when the specified 'boolBehavior' is false at those times, in which case
  // the occurance is omitted.
  template <typename A>
  static Behavior<boost::optional<A>> when(
      const Behavior<bool>& behavior,
      const Behavior<boost::optional<A>>& event);

  // Return the result of converting an 'Event (Op a)' into an 'Event a',
  // removing all none instances.
  template <typename A>
  static Behavior<boost::optional<A>> just(
      const Behavior<boost::optional<boost::optional<A>>>& event);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Behavior<T> EventUtil::step(const T& t0,
                            const Behavior<boost::optional<T>>& event) {
  sfrp::Wormhole<T> wormhole(t0);
  return wormhole.setInputBehavior(sfrp::BehaviorMap()(
      &sboost::OptionalUtil::getValueOr<T>, event, wormhole.outputBehavior()));
}
template <typename T>
static Behavior<boost::optional<T>> EventUtil::never() {
  return sfrp::BehaviorUtil::always(boost::optional<T>());
}
template <typename A>
Behavior<boost::optional<A>> EventUtil::merge(
    const Behavior<boost::optional<A>>& leftEvent,
    const Behavior<boost::optional<A>>& rightEvent) {
  return sfrp::BehaviorMap()(
      sboost::OptionalUtil::alternative<A>, leftEvent, rightEvent);
}

template <typename Function, typename A>
Behavior<boost::optional<A>> EventUtil::mergeWith(
    Function function,
    const Behavior<boost::optional<A>>& leftEvent,
    const Behavior<boost::optional<A>>& rightEvent) {
  return sfrp::BehaviorMap()([function](boost::optional<A> lhs,
                                        boost::optional<A> rhs) {
                               return lhs && rhs ? function(*lhs, *rhs)
                                                 : lhs ? *lhs : rhs;
                             },
                             leftEvent,
                             rightEvent);
}

template <typename Function, typename A>
static Behavior<boost::optional<A>> EventUtil::filter(
    Function function,
    const Behavior<boost::optional<A>>& event) {
  return sfrp::BehaviorMap()([function](boost::optional<A> occurance) {
                               return occurance && function(*occurance)
                                          ? occurance
                                          : boost::none;
                             },
                             event);
}
template <typename Function, typename A>
Behavior<boost::optional<A>> EventUtil::accumulate(
    A initialValue,
    const Behavior<boost::optional<Function>> accumulator)
{
  Wormhole<A> valueWormhole = Wormhole<A>(initialValue);
  sfrp::Behavior<boost::optional<A>> result =
      sfrp::EventMap()([](Function f, A a)->A { return f(a); },
                       accumulator,
                       valueWormhole.outputBehavior());

  // The use of curtail here ensures that the wormhole input behavior is getting
  // pulled as frequently as the result is.
  return sfrp::BehaviorUtil::curtail(
      result,
      valueWormhole.setInputBehavior(EventUtil::step(initialValue, result)));
}

template <typename A, typename B>
Behavior<boost::optional<std::pair<A, B>>> EventUtil::snapshot(
    const Behavior<B>& behavior,
    const Behavior<boost::optional<A>>& event) {
  return sfrp::BehaviorMap()([](const B & b, const boost::optional<A> & opA) {
                               return opA ? boost::make_optional(
                                                std::make_pair(*opA, b))
                                          : boost::none;
                             },
                             behavior,
                             event);
}
template <typename A, typename B>
static Behavior<boost::optional<B>> EventUtil::snapshot_(
    const Behavior<B>& behavior,
    const Behavior<boost::optional<A>>& event) {
  return sfrp::BehaviorMap()([](const B & b, const boost::optional<A> & opA) {
                               return opA ? boost::make_optional(b)
                                          : boost::none;
                             },
                             behavior,
                             event);
}

template <typename A>
static Behavior<boost::optional<A>> EventUtil::when(
    const Behavior<bool>& b,
    const Behavior<boost::optional<A>>& a) {
  return sfrp::BehaviorOperators::ifThenElse(
      b, a, sfrp::BehaviorUtil::always(boost::optional<A>()));
}

template <typename A>
static Behavior<boost::optional<A>> EventUtil::just(
    const Behavior<boost::optional<boost::optional<A>>>& event) {
  return BehaviorMap()(sboost::OptionalUtil_Join(), event);
}

}
#endif

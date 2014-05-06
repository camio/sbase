#ifndef SFRP_EVENTUTIL_HPP_
#define SFRP_EVENTUTIL_HPP_

#include <boost/optional.hpp>
#include <sboost/optionalutil.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/wormhole.hpp>
#include <utility>  // std::make_pair

namespace sfrp {
struct EventUtil {

  // Turn an event into a behavior. Whatever the value of the specified 'event'
  // is, the behavior will have that value until the next occurance. The
  // behaviors initial value is defined by the specified 't0'.
  template <typename T>
  static Behavior<T> step(const T& t0,
                          const Behavior<boost::optional<T>>& event);

  // Merge the specified 'leftEvent' and 'rightEvent' events. Preference is
  // given to 'leftEvent' if there is an occurance at the same time.
  template <typename A>
  static Behavior<boost::optional<A>> merge(
      const Behavior<boost::optional<A>>& leftEvent,
      const Behavior<boost::optional<A>>& rightEvent);

  template <typename A, typename B>
  static Behavior<boost::optional<std::pair<A, B>>> snapshot(
      const Behavior<B>& behavior,
      const Behavior<boost::optional<A>>& event);

  //..
  //  pmSnapshot_ b e = map snd (pmSnapshot b e)
  //..
  template <typename A, typename B>
  static Behavior<boost::optional<B>> snapshot_(
      const Behavior<B>& b,
      const Behavior<boost::optional<A>>& e);
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
template <typename A>
Behavior<boost::optional<A>> EventUtil::merge(
    const Behavior<boost::optional<A>>& leftEvent,
    const Behavior<boost::optional<A>>& rightEvent) {
  return sfrp::BehaviorMap()(sboost::OptionalUtil::alternative<A>, a, b);
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
    const Behavior<B>& b,
    const Behavior<boost::optional<A>>& e) {
  return sfrp::BehaviorMap()(detail::snapshot_Helper<A, B> [](
                                 const B & b, const boost::optional<A> & opA) {
                               return opA ? boost::make_optional(b)
                                          : boost::none;
                             },
                             b,
                             e);
}
}
#endif

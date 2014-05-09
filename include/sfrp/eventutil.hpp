#ifndef SFRP_EVENTUTIL_HPP_
#define SFRP_EVENTUTIL_HPP_

//@PURPOSE: Provide utility operations for event-like 'Behavior' objects.
//
//@CLASSES:
//  sfrp::EventUtil: namespace for event utility operations.
//
//@SEE_ALSO: sfrp_eventmap, sfrp_triggerutil
//
//@DESCRIPTION: This component provides a single namespace class,
// 'EventUtil', which has several utility functions operating on event-like
// 'Behavior' objects.
//
// Events
// ------
// Semantically, events are identical to behaviors with optional values. At the
// times when the behavior has a not-none event, we say the event has an
// occurance. Although this is a very clean semantic definition, we would like
// to, in practice, count the number of occurances and perform other similar
// functions. Because of the way we defined events, we quickly get into the
// situation where some events have infinite occurances between two specific
// times.
//
// To work around that issue, we introduce the concept of the 'pull' event.
// This event occurs at every time that 'pull' is called with. This event is
// well defined from the semantics derivation of 'Behavior's. Using this 'pull'
// event we can sidestep the infinite occurances issue. A function which counts
// event occurances, for instance, only considers occurances that happen at the
// same time as 'pull' events. In this sense, all of our counting event
// functions can be considered precisely defined approximations.
//
// The astute reader will question how one might handle real-world events such
// as mouse clicks. If a mouse click doesn't occur at the same moment as a
// pull, will it just be ignored? For this situation, instead of defining the
// mouse events to occur the instant a mouse is pressed, we define a mouse
// event to occur from the instant it is pressed until occurance of the next
// 'pull' event. In this way we can guarentee that all real-world events are
// captured. See the sfrp_triggerutil component for construction of these kinds
// of behaviors.
//
// For events defined with 'sfrp::BehaviorUtil::pure', the problem of counting
// occurances is undecidable so we are unfortunately left with our precisely
// defined approximations.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Step color choosing
// - - - - - - - - - - - - - - -
// Say we have an event of type 'sfrp::Behavior<boost::optional<Color>>' called
// 'colorChanges'. This event occurs whenever the user chooses a new color
// through some kind widget. We would like to have a box on the screen which is
// filled with the currently selected color.
//
// To do this, we can use the 'step()' function to define a behavior out of the
// 'colorChanges' event.
//..
//  sfrp::Behavior<Color> color = sfrp::EventUtil::step( white(), colorChanges );
//..
// In essence, the resulting 'color' behavior starts out as white and changes
// at every occurance of 'colorChanges'. The difference between 'color' and
// 'colorChanges' is that the former has a defined color at every point in time
// where the later only has a color at certain points in time.
//
// We can now use the 'color' behavior as we would any other color behavior to
// control a drawing;
//..
//  sfrp::Behavior<Drawing> drawing = /* use 'color' as the drawing's color */;
//..
//
// Example 2: Snapshot color selection
// - - - - - - - - - - - - - - - - - -
// Moving backwards from our previous example, lets say we have a color swatch
// on the screen and we wish to have an event that carries which color the user
// has selected on the swatch.
//
// Lets say we have the following setup
// - 'sfrp::Behavior<boost::optional<int>> mouseClick'. An event corresponding
//    to when the user clicks the mouse of Lets say that the 'int' carried at
//    event occurances is the mouse button used, something we don't care about.
// - 'sfrp::Behavior<Point2D> mousePos' which corresponds to the
//   mouse position.
// - A function 'function<Color (Point2D)> posToColor' that converts a
//   coordinate on the screen to that pixel's color.
//
// What we'd really like to get is a 'sfrp::Behavior<boost::optional<Point2D>>'
// object that corresponds to the events when the user clicks a color. We
// construct this as follows.
//..
//  sfrp::Behavior<boost::optional<Color>> colorChanges = sfrp::EventMap()(
//      posToColor, sfrp::EventUtil::snapshot_(mousePos, mouseClick));
//..
// First, we are using the 'snapshot_()' function to make our original
// 'mouseClick' event hold the mouse position instead of the mouse button at
// each occurance. Once we had a 'sfrp::Behavior<boost::optional<Point2D>>'
// event, we converted it into a 'Color' event using the 'sfrp::EventMap'
// functor in combination with the 'posToColor' function.
//
// Example 3: Counting the score
// - - - - - - - - - - - - - - -
// Lets say we have a 'sfrp::Behavior<boost::optional<int>>' event that occurs
// whenever the user's score gets modified in a game. The 'int' each event
// carries corresponds to how many points they either gained or lost at that
// point in time.
//
// If we wish to get their current total score, we can make use of the
// 'accumulate()' function. Lets take a look and then explain.
//..
//  sfrp::Behavior<int> score = sfrp::EventUtil::step(
//      0,
//      sfrp::EventUtil::accumulate(
//          0,
//          sfrp::EventMap()([](int change) {
//                             return boost::function<int(int)>([change](
//                                 int total) { return total + change; });
//                           },
//                           scoreChanges)));
//..
// The first thing to look at is the 'EventMap' call on the inside. What it
// does is convert value in 'scoreChanges' into a function that takes in an int
// and adds whatever the value was. Its type is
// 'sfrp::Behavior<boost::optional<boost::function<int(int)>>>'. Events with
// single argument functions is exactly what the 'accumulate()' function
// requires. 'accumulate()' is then called to apply the first occurance's
// function to '0', the second occurance's function to the result of that, and
// so on. The result of accumulate is type
// 'sfrp::Behavior<boost::optional<int>>' and corresponds to the events when
// the total changes which carry the new total.
//
// Finally, the 'step()' function is used to convert the event into a behavior
// which could then be used for some sort of display.
//
// The use of 'accumulate()' is somewhat low-level in this example. It would
// likely be preferrable to use a 'sum()' function that will work with any
// summable event which is written using 'accumulate()'.
//
// Example 4: Merging score events
// - - - - - - - - - - - - - - - -
// Merging events is a straightforward operation. Say we have two seperate
// events that correspond to changes in the user's score. Both are of type
// 'sfrp::Behavior<boost::optional<int>>'. If we want to collect all scoring
// occurances, we can use one of the merge functions.
//..
//  sfrp::Behavior<boost::optional<int>> allScoreChanges =
//      sfrp::EventUtil::merge(powerupScoreChanges, enemyDestroyedScoreChanges);
//..
// The above implementation will work, but the semantics of 'merge()' imply that
// if there is a power-up score change at the same time as an enemy destroyed
// score change, the former will take preference. What we probably want is for
// the scores to be added together in this case. For this, we can use the
// 'mergeWith()' function instead.
//..
//  sfrp::Behavior<boost::optional<int>> allScoreChanges =
//      sfrp::EventUtil::mergeWith([](int a, int b) { return a + b; },
//                                 powerupScoreChanges,
//                                 enemyDestroyedScoreChanges);
//..
// The 'mergeWith()' function takes in a function argument which is applied
// when two occurences happen at the same time.

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

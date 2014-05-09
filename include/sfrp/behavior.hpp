#ifndef SFRP_BEHAVIOR_HPP_
#define SFRP_BEHAVIOR_HPP_

//@PURPOSE: Provide a time/value function representation for FRP.
//
//@CLASSES:
//  sfrp::Behavior: FRP time function representation
//
//@DESCRIPTION: This component provides a single class, 'Behavior', which
// represents time functions for development with Functional Reactive
// Programming (FRP).
//
// The class provides both a clear dentotational semantics for use when
// creating FRP graphs, and a useful operational semantics which is used to
// interface networks with the rest of the system.
//
// Semantics
// ---------
// We describe our mathematical semantics using Agda notation.
//
// Let 'T' be a the type for time. A behavior is a pair of an end time, 't',
// and a function, 'f', which ranges from time '0' to time 't'. 'α' is the
// codomain of the function.
//..
//  μ⟦Behavior<α>⟧ : ( t : T, f : (0,t) → α )
//..
// The main function we use to pull values out of behaviors is called pull. It
// takes in a time 't', a behavior of type α, and returns, optionally, a value
// of type 'α'.
//..
//  μ⟦pull⟧ : T → {α : Set} → ( tEnd : T, f : (0,tEnd) → α ) → Maybe α
//..
// 'pull' is defined as follows.
//..
//  μ⟦pull⟧ = λt _ → if t < tEnd
//                     then Nothing
//                     else Just (f t)
//..
// The 'Behavior::pull()' member function closely relates to our meaning of
// 'pull' above, except it is not a pure function. Instead, it has some
// preconditions that ensure that it is always called with increasing values of
// 't'. This added condition to the pull procedure allows past information to
// be garbage collected as appropriate. For more information on exactly how
// this came about, see the "Semantics to Implementation Derivation" section.
//
// Semantics to Implementation Derivation
// --------------------------------------
// In this section, we outline the process used to derive the semantics
// mentioned above into our C++ implementation. It assumes a basic
// understanding of Agda syntax and the semantics derivation process.
//
// T is a type representing time. It can hold non-negative real numbers. A ≅ B
// indicates that A is isomorphic to B.
//..
//  Mv α = ( t : T
//         , f : (0,t) → α
//         )
//..
// 'Mv' is the base semantics for moves. It consists of an end time, and a
// function over 0 to that end time. For 3D moves 'α' would be 'ℝ³'. For 5D
// moves, 'α' might be '(ℝ³, O²)'.
//
// A problem with using 'Mv' as the implementation of move, is that we often
// would like to know some 'f' mappings before 't', the final time, is known.
// An example would be a jogging movement in progress before it is known when
// the stop button is pressed. We workaround this issue by encoding the end
// time into the function
//..
//  MvE α = ( f : T → Maybe α
//          ,   (t₀ t₁ : T)
//            → t₀ ≤ t₁
//            → f t₀ ≡ Nothing
//            → f t₁ ≡ Nothing
//          )
//..
// The second element of the tuple is a proof that when the result of 'f t' is
// 'Nothing', all subsequent values of 't' also have a result of 'Nothing'. By
// our cursory look, 'MvE' is isomorphic to 'Mv'.
//
// 'MvE', used as an implementation, still has a defect that needs fixing. If
// our function is defined by arbitrary mouse movements, which it is in the
// case of a velocity graph, then storage for 'MvE' would grow with time. This
// is undesirable behavior.
//
// Our final implementation, 'MvF', utilizes the idea that a function returns
// the value and a partial function akin to a continuation.
//
// F₀ is our first attempt at designing 'MvF':
//..
//  F₀ : Set → T → Set
//  F₀ α t = u : T
//         → u > t
//         → (Maybe α, F u)
//..
// Conceptually, 'F₀' takes in a type and a time, and returns a continuation
// function type. "F₀ α 0" is a good start for MvF. This function ensures that
// the "continuation" function is only over values greater than 'u'. This way
// of definition resolves the space leak because old 'F₀ α t' values can be
// garbage collected.
//
// We still need to incorporate the proof similar to that included in 'MvE'.
//..
// F₁ : Set → T → Set
// F₁ α t = ( f : ( u : T
//                → u > t
//                → (Maybe α, F u)
//                )
//          ,   (t₀ t₁ : T)
//            → (p₀ : t₀ > t)
//            → (p₁ : t₁ > t)
//            → t₀ ≤ t₁
//            → fst (f t₀ p₀) ≡ Nothing
//            → fst (f t₁ p₁) ≡ Nothing
//          )
//..
//
// By recursion we see that this associated proof applies to all continuations.
//
// At this point we still don't have something isomorphic to MvE, although we
// have a monomorphism (we can write an injective function from MvE α to F₁ α
// 0). The reason we don't have a homomorphism is because we don't include a
// condition that ensures that the continuations of 'f' are consistent with
// 'f'. We add this condition below.
//..
// F₂ : Set → T → Set
// F₂ α t = ( f : ( u : T
//                → u > t
//                → (Maybe α, F u)
//                )
//          ,   (t₀ t₁ : T)
//            → (p₀ : t₀ > t)
//            → (p₁ : t₁ > t)
//            → t₀ ≤ t₁
//            → fst (f t₀ p₀) ≡ Nothing
//            → fst (f t₁ p₁) ≡ Nothing
//          ,   (t₀ t₁ : T)
//            → (p : t₀ < t₁)
//            → (p₀ : t₀ > t)
//            → (p₁ : t₁ > t)
//            → (fst (f t₀ p₀)) t₁ p ≡ f t₁ p₁
//          )
//..
// Define 'MvF α = F₂ α 0'.
//
// Since we have Mv ≅ MvE and MvE ≅ MvF, then we also have Mv ≅ MvF. We are now
// very close to a valid implementation based on MvF.
//..
//  template< typename A >
//  struct Behavior {
//    typedef function<optional<A> (Time)> Impl;
//    Impl impl;
//  };
//..
// 'Behavior' uses a boost "function", more properly called procedure, as its
// implementation. We look to discover the meaning of calling function given a
// monadic state context.
//..
//  μ⟦ Move<A> m = β ⟧ = m ← newRef μ⟦β⟧
//  μ⟦ β ⟧ : MvF A
//..
// 'β' represents an expression that constructs a 'Behavior<A>' and we leave it
// abstract.  Creation of a 'Behavior<A>' in C++ corresponds to creating a
// reference to a 'MvF A'.
//..
//  μ⟦ m.impl(t₁) ⟧ = (f₀,_,_) ← get m (where m's content's type is F₂ α t₀)
//                    if (p : μ⟦t₁⟧ > t₀)
//                      then
//                        let (v,mVal′) = f₀ μ⟦t₁⟧ p
//                        set m mVal′
//                        case m of Nothing → boost::none
//                                  Just a → a
//                      else
//                        error "assertion failed"
//..
// A 'Behavior<A> m', when initially created, is isomorphic to 'Mv μ⟦A⟧'. After 'm' has
// 'impl' called for the first time, it is isomorphic to the "continuation"
// functions of the form 'F₂ α t'.
//
// We also want to deal with copy semantics.
//
// When b is defined elsewhere, 'Behavior<A> m = n' implies that 'm' and 'n'
// refer to the same reference. Implementation wise, we handle this by use of a
// shared pointer.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Always time function
// - - - - - - - - - - - - - - - -
// In this example, we illustrate how to use the 'fromValuePullFunc' function
// to construct behaviors with the sematics we desire.
//
// First, we create a behavior that has the value '3' for all times. Because
// this behavior never ends, we construct our function to always return
// 'make_optional(3)'. The time argument is ignored.
//..
//  sfrp::Behavior<int> alwaysThree = sfrp::Behavior<int>::fromValuePullFunc([](
//      double time) { return boost::make_optional(3); });
//..
// Even though the 'fromValuePullFunc' gives some precise explainations of how
// its argument will be used, it is no harm to have values defined for
// arguments of our function that won't be used (for example, at time '-1.0').
//
// If we want to extend this to a general purpose 'always' function, we can do
// so by making a function template.
//..
//  template <typename Value>
//  sfrp::Behavior<Value> always(const Value& value) {
//    return sfrp::Behavior<Value>::fromValuePullFunc([value](double time) {
//      return boost::make_optional(value);
//    });
//  }
//..
// Here we keep hold of the value in the lambda that is formed. We can use this
// 'always' function now to simplify our alwaysThree function.
//..
//  sfrp::Behavior<int> alwaysThree = always( 3 );
//..
// Generally speaking, FRP graphs are constructed using higher level functions
// like 'always' instead of the lower-level 'fromValuePullFunc'.
//
// The 'always' function is defined for general use in sfrp_behaviorutil.
//
// Example 2: Composition
// - - - - - - - - - - - 
// One of the most powerful features of FRP is the ability to compose simple
// behaviors into more complex ones. Here we define a '+' operator that can be
// used to add behaviors together.
//
// The key idea here is to use the 'pull' function inside the implementation of
// the argument to 'fromValuePullFunc'. Checking the result ensures that the
// '+' of behaviors is only defined when the two arguments are defined.
//..
//  sfrp::Behavior<int> operator+( sfrp::Behavior<int> lhs,
//                                 sfrp::Behavior<int> rhs ) {
//    return sfrp::Behavior<int>::fromValuePullFunc([lhs, rhs](double time)
//                                                      ->boost::optional<int> {
//      boost::optional<int> lhsVal = lhs.pull(time);
//      boost::optional<int> rhsVal = rhs.pull(time);
//      if (lhsVal && rhsVal)
//        return boost::make_optional(*lhsVal + *rhsVal);
//      else
//        return boost::none;
//    });
//  }
//..
// The use of 'fromValuePullFunc' and 'pull' here are too low level for
// most composition operations including this one. See the 'lift()' functions
// available in behavior_util for better ways to accomplish this task.
//
// Example 3: Mouse position behavior
// - - - - - - - - - - - - - - - - -
// Interaction, such as having the ability to construct a behavior that
// represents the mouse position, is an important use case for functional
// reactive programming. In this example we see how we would create such a
// behavior.
//
// First, assume we have a global function that returns the current mouse
// position.
//..
//  std::pair<int, int> getMousePos();
//..
// We construct our behavior by directly using this global variable.
//..
//  sfrp::Behavior<std::pair<int, int>> mousePos =
//      sfrp::Behavior<std::pair<int, int>>::fromValuePullFunc([](double time) {
//        return boost::make_optional(getMousePos());
//      });
//..
// Of course, we need to discuss what time means in this case. This definition
// of 'mousePos' has built in it the expectation that 'time' will always
// correspond to the current time on a computer.
//
// Although this is safe to assume in most cases, it may be necessary to allow
// for the pulling of the mouse position at times prior to the present.
//
// To accomplish this a "push-pull" mechanism must be put into place.
//
// We first construct and initialize a queue of mouse position values. This
// will be the buffer between the execution which polls the mouse position and
// the execution which pulls out values.
//
// It is important that we initialize it with a value in case the 'pull' comes
// before our filler execution first adds a value.
//..
//  boost::shared_ptr<
//      std::list<sfrp::CachedPull<std::pair<int, int>>>> mousePosQueue =
//      boost::make_shared<std::list<sfrp::CachedPull<std::pair<int, int>>>>();
//  // Intialize the queue
//  mousePosQueue->emplace_front(
//      sfrp::CachedPull<std::pair<int, int>>(currentTime(), getMousePos()));
//..
//  Our 'push' execution will repeately add a mouse position value to the queue.
//..
//  mousePosQueue->emplace_front(
//      sfrp::CachedPull<std::pair<int, int>>(currentTime(), getMousePos()));
//..
//  The definition of the mouse position behavior is as follows. As a value is
//  requested, the queue is cleared of unneeded values.
//..
//  sfrp::Behavior<std::pair<int, int>> mousePos =
//      sfrp::Behavior<std::pair<int, int>>::fromValuePullFunc([mousePosQueue](
//          double time) {
//        // Look in our queue for the first entry with a time less than 'time'.
//        auto iterator =
//            std::find_if(mousePosQueue->begin(),
//                         mousePosQueue->end(),
//                         [time](sfrp::CachedPull<std::pair<int, int>> val) {
//              return val.time() <= time;
//            });
//
//        // We've requested a time before our initial value. This indicates a
//        // programming error.
//        assert(iterator != mousePosQueue->end());
//
//        // Clear the cache we don't need. We can do this because we know all
//        // future calls will have an argument greater than 'time'.
//        if (std::next(iterator) != mousePosQueue->end())
//          mousePosQueue->erase(std::next(iterator), mousePosQueue->end());
//
//        return boost::make_optional(iterator->value());
//      });
//..
// One thing to keep in mind with this push-pull strategy is that the pull
// code is racing against the pull code to keep the buffer from exploding in
// size. One might desire to remedy this possible memory explosion by ensuring
// that the buffer never grows beyond a certain size at the expense of
// resolution.
//
// Both the pull method and the push-pull method are correct semantically
// speaking. The way the time functions are defined is the only thing that
// changes. The intuitive concept of mouse position as related to time,
// however, is more accurately modeled by the push-pull mechanism.
//
// Example 4: Retrieving Values
// - - - - - - - - - - - - - -
// Once a FRP graph has been set up, values are retrieved using the 'pull()'
// function of behaviors.
//
// Presuming we have a 'drawingBehavior' value of type
// 'sfrp::Behavior<Drawing>', we could write an animated rendering routine as
// follows:
//..
//  const std::time_t initialCalendarTime = std::time(0);
//  std::time_t currentCalendarTime = initialCalendarTime;
//  for (;;) {
//    const double time = std::difftime( currentCalendarTime, initialCalendarTime );
//    boost::optional<Drawing> drawing = drawingBehavior.pull(time);
//    if( drawing )
//      draw( *drawing );
//    else
//      // Behavior no longer defined
//      return;
//    currentCalendarTime = std::time(0);
//  }
//..
// The drawback of this fast-as-possible pull method is that the CPU will be
// used to its full capacity even though it may not be necessary to have frame
// rates beyond 60Hz. In this version we ensure that the frame rate will not
// have a higher frequency than 60Hz.
//..
//  const std::time_t initialCalendarTime = std::time(0);
//  std::time_t currentCalendarTime = initialCalendarTime;
//  for (;;) {
//    const double time = std::difftime( currentCalendarTime, initialCalendarTime );
//    boost::optional<Drawing> drawing = drawingBehavior.pull(time);
//    if( drawing )
//      draw( *drawing );
//    else
//      // Behavior no longer defined
//      return;
//
//    currentCalendarTime = std::time(0);
//    const double nextTime = std::difftime( currentCalendarTime, initialCalendarTime );
//    const double minimumSecondsBetweenFrames = 1.0/60.0;
//    if( minimumSecondsBetweenFrames > nextTime - time ) {
//      std::this_thread::sleep_for(std::chrono::milliseconds(
//          int(1000 * (minimumSecondsBetweenFrames - (nextTime - time)))));
//      currentCalendarTime = std::time(0);
//    }
//  }
//..

#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/cachedincreasingpartialtimefunction.hpp>

namespace sfrp {

// This class implements a partial function from time to the specified 'Value'
// type. The function is defined from time '0' to some end time greater than
// '0'. Behaviors that are not defined at time '0', or any subsequent time, are
// permitted.
template <typename Value>
struct Behavior {

  // The codomain of this partial time function.
  typedef Value type;

  // Create a 'Behavior' object that is not defined at any time.
  Behavior();

  // Create a 'Behavior' object equivelent to the specified 'behavior'.
  Behavior(Behavior<Value>&& behavior);

  // Return the value of this partial time function at the specified 'time' if
  // it is defined, otherwise return 'boost::none'. If a previous call to
  // 'pull()' returns 'boost::none', so will all subsequent calls.  The
  // behavior is undefined unless 'pull()' is being called for the first time
  // or the previous call to 'pull()' used a time argument that is less than
  // 'time'. The behavior is also undefined unless 'time >= 0.0'.
  //
  // Note that the aformentioned preconditions imply that 'pull()' is called
  // with increasing time arguments and that after 'pull()' returns
  // 'boost::none', it will return 'boost::none' thereafter.
  boost::optional<Value> pull(const double time) const;

  // Create a new 'Behavior<Value>' object from the 'valuePullFunc' function.
  // The behavior is undefined unless 'valuePullFunc' returns a value when it
  // is called once and is defined with increasing argument values as long as
  // the previous call didn't return 'boost::none'.
  static Behavior<Value> fromValuePullFunc(
      boost::function<boost::optional<Value>(double)> valuePullFunc);

 private:
  mutable boost::shared_ptr<CachedIncreasingPartialTimeFunction<Value>> m_timeFunction;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A>
Behavior<A> Behavior<A>::fromValuePullFunc(
    boost::function<boost::optional<A>(double)> valuePullFunc) {
  Behavior<A> result;
  result.m_timeFunction =
      boost::make_shared<CachedIncreasingPartialTimeFunction<A>>(std::move(valuePullFunc));
  return result;
}

template <typename A>
Behavior<A>::Behavior()
    : m_timeFunction() {}

template <typename A>
boost::optional<A> Behavior<A>::pull(const double time) const {
  if (m_timeFunction) {
    boost::optional<A> result = m_timeFunction->pull(time);
    if (!result)
      m_timeFunction.reset();
    return result;
  } else
    return boost::none;
}

template <typename A>
Behavior<A>::Behavior(Behavior<A>&& behavior)
    : m_timeFunction(std::move(behavior.m_timeFunction)) {}
}
#endif

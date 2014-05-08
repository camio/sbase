#ifndef SFRP_BEHAVIOROPERATORS_HPP_
#define SFRP_BEHAVIOROPERATORS_HPP_

//@PURPOSE: Provide overloads of C++ operators for behaviors.
//
//@CLASSES:
//  sfrp::BehaviorOperators: namespace for operators that can't be overloaded
//
//@DESCRIPTION: This component provides operator overloads, and one free
// function 'ifThenElse', that can be applied to behaviors. All these functions
// fit the general pattern where the values of the resulting behavior, for every
// time 't', are the operator applied to the values of the argument behaviors at
// time 't'.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Adding behaviors
// - - - - - - - - - - - - - -
// Say we have two 'sfrp::Behavior<double>' behaviors called 'a', and 'b'. We
// can compute a behavior consisting of the sum of their values at every time
// step using the '+' operator overload.
//..
//  sfrp::Behavior<double> c = a + b;
//..
// The other operator overloads and functions work in a similar way.

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <scpp/operators.hpp>

namespace sfrp {

// Return the behavior where for every time t, the value is the specified 'lhs'
// behavior at that time minus the specified 'rhs' behavior at that time. The
// resulting behavior is defined wherever both 'lhs' and 'rhs' are defined.
template <typename A, typename B>
inline auto operator-(const Behavior<A>& lhs, const Behavior<B>& rhs)
    -> decltype(BehaviorMap()(scpp::Operators::minus<A, B>, lhs,rhs));

// Return the behavior where for every time t, the value is the specified 'lhs'
// behavior at that time plus the specified 'rhs' behavior at that time. The
// resulting behavior is defined wherever both 'lhs' and 'rhs' are defined.
template <typename A, typename B>
inline auto operator+(const Behavior<A>& lhs, const Behavior<B>& rhs)
    -> decltype(BehaviorMap()(scpp::Operators::plus<A, B>, lhs, rhs));

// Return the behavior where for every time t, the value is the negation of the
// specified 'behavior' at that time.
template <typename A>
inline auto operator-(const Behavior<A>& behavior)
    -> decltype(BehaviorMap()(scpp::Operators::negate<A>, behavior));

// Return the behavior where for every time t, the value is the specified 'lhs'
// behavior at that logically and-ed with the specified 'rhs' behavior at that
// time. The resulting behavior is defined wherever both 'lhs' and 'rhs' are
// defined.
template <typename T>
Behavior<T> operator&&(const Behavior<T>& lhs, const Behavior<T>& rhs);

// Return the behavior where for every time t, the value is the specified 'lhs'
// behavior at that logically or-ed with the specified 'rhs' behavior at that
// time. The resulting behavior is defined wherever both 'lhs' and 'rhs' are
// defined.
template <typename T>
Behavior<T> operator||(const Behavior<T>& lhs, const Behavior<T>& rhs);

// Return the behavior where for every time t, the value is the logical-not of
// the specified 'behavior' at that time.
template <typename T>
Behavior<T> operator!(const Behavior<T>& behavior);

// This class provides a namespace for operators that can be applied to
// behaviors, but cannot be overloaded.
struct BehaviorOperators {
  // Return a behavior that is the specified 'trueCase' whenever 'comparison' is
  // 'true' and 'falseCase' whenver 'comparison' is 'false'.
  //
  // Note that we made this a free function because we cannot overload the '?:'
  // operator in C++.
  template <typename A>
  static Behavior<A> ifThenElse(const Behavior<bool>& comparison,
                                const Behavior<A>& trueCase,
                                const Behavior<A>& falseCase);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A, typename B>
inline auto operator-(const Behavior<A>& a, const Behavior<B>& b)
    -> decltype(BehaviorMap()(scpp::Operators::minus<A, B>, a, b)) {
  return BehaviorMap()(scpp::Operators::minus<A, B>, a, b);
}

template <typename A, typename B>
inline auto operator+(const Behavior<A>& a, const Behavior<B>& b)
    -> decltype(BehaviorMap()(scpp::Operators::plus<A, B>, a, b)) {
  return BehaviorMap()(scpp::Operators::plus<A, B>, a, b);
}

template <typename A>
inline auto operator-(const Behavior<A>& a)
    -> decltype(BehaviorMap()(scpp::Operators::negate<A>, a)) {
  return BehaviorMap()(scpp::Operators::negate<A>, a);
}
template <typename T>
Behavior<T> operator!(const Behavior<T>& a) {
  return BehaviorMap()(scpp::Operators::logicalNot<T>, a);
}

template <typename T>
Behavior<T> operator||(const Behavior<T>& a, const Behavior<T>& b) {
  return BehaviorMap()(scpp::Operators::logicalOr<T>, a, b);
}
template <typename T>
Behavior<T> operator&&(const Behavior<T>& a, const Behavior<T>& b) {
  return BehaviorMap()(scpp::Operators::logicalAnd<T>, a, b);
}

template <typename A>
Behavior<A> BehaviorOperators::ifThenElse(const Behavior<bool>& comparison,
                                          const Behavior<A>& trueCase,
                                          const Behavior<A>& falseCase) {
  return sfrp::BehaviorMap()(
      scpp::Operators::ifThenElse<A>, comparison, trueCase, falseCase);
}
}

#endif

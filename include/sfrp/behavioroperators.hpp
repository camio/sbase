#ifndef SFRP_BEHAVIOROPERATORS_HPP_
#define SFRP_BEHAVIOROPERATORS_HPP_

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <scpp/operators.hpp>

namespace sfrp {

template <typename A, typename B>
inline auto operator-(const Behavior<A>& a, const Behavior<B>& b)
    -> decltype(BehaviorMap()(scpp::Operators::minus<A, B>, a, b));

template <typename A, typename B>
inline auto operator+(const Behavior<A>& a, const Behavior<B>& b)
    -> decltype(BehaviorMap()(scpp::Operators::plus<A, B>, a, b));

template <typename A>
inline auto operator-(const Behavior<A>& a)
    -> decltype(BehaviorMap()(scpp::Operators::negate<A>, a));

template <typename T>
Behavior<T> operator&&(const Behavior<T>& a, const Behavior<T>& b);

template <typename T>
Behavior<T> operator||(const Behavior<T>& a, const Behavior<T>& b);

template <typename T>
Behavior<T> operator!(const Behavior<T>& a);

// Return a behavior that is the specified 'trueCase' whenever 'comparison' is
// 'true' and 'falseCase' whenver 'comparison' is 'false'.
//
// Note that we made this a free function because we cannot overload the '?:'
// operator in C++.
template <typename A>
Behavior<A> ifThenElse(const Behavior<bool>& comparison,
                const Behavior<A>& trueCase,
                const Behavior<A>& falseCase);

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
Behavior<A> ifThenElse(const Behavior<bool>& comparison,
                const Behavior<A>& trueCase,
                const Behavior<A>& falseCase) {
  return sfrp::BehaviorMap()(
      scpp::Operators::ifThenElse<A>, comparison, trueCase, falseCase);
}
}

#endif

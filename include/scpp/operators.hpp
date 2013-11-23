#ifndef SCPP_OPERATORS_HPP_
#define SCPP_OPERATORS_HPP_

//@PURPOSE: Provide named versions of C++ operators.
//
//@CLASSES:
//  scpp::Operators: named C++ operators namespace
//
//@DESCRIPTION: This component provides functions that are equivelent to common
// C++ operators, such as '+' and '=='. These are generally useful when these
// operators need to be passed to higher order functions (functions that take
// functions as an argument). Although the standard C++ 'functional' include
// provides this functionality, it does so with functors instead of functions
// which are incompatible with some higher order functions.
//
// The naming conventions of these functions intentionally mimics those of the
// standard C++ 'functional' include while maintaining our own naming
// conventions.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Simply call the higher order function with one of these to get the intended
// result.

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace scpp {

// This class provides a namespace for named versions of C++ operators.
struct Operators {

  // Return 'a + b'.
  template <typename A, typename B>
  static auto plus(const A& a, const B& b) -> typename boost::remove_reference<
      typename boost::remove_const<decltype(a + b)>::type>::type;

  // Return 'a - b'.
  template <typename A, typename B>
  static auto minus(const A& a, const B& b) -> typename boost::remove_reference<
      typename boost::remove_const<decltype(a - b)>::type>::type;

  // Return 'a * b'.
  template <typename A, typename B>
  static auto multiplies(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a* b)>::type>::type;

  // Return 'a / b'.
  template <typename A, typename B>
  static auto divides(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a / b)>::type>::type;

  // Return 'a % b'.
  template <typename A, typename B>
  static auto modulus(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a % b)>::type>::type;

  // Return '-a'.
  template <typename A>
  static auto negate(const A& a) -> typename boost::remove_reference<
      typename boost::remove_const<decltype(-a)>::type>::type;

  // Return 'a == b'.
  template <typename A, typename B>
  static auto equalTo(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a == b)>::type>::type;

  // Return 'a != b'.
  template <typename A, typename B>
  static auto notEqualTo(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a != b)>::type>::type;

  // Return 'a > b'.
  template <typename A, typename B>
  static auto greater(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a > b)>::type>::type;

  // Return 'a < b'.
  template <typename A, typename B>
  static auto less(const A& a, const B& b) -> typename boost::remove_reference<
      typename boost::remove_const<decltype(a < b)>::type>::type;

  // Return 'a >= b'.
  template <typename A, typename B>
  static auto greaterEqual(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a >= b)>::type>::type;

  // Return 'a <= b'.
  template <typename A, typename B>
  static auto lessEqual(const A& a, const B& b)
      -> typename boost::remove_reference<
            typename boost::remove_const<decltype(a <= b)>::type>::type;

  // Return 'a && b'.
  template <typename T>
  static T logicalAnd(const T& a, const T& b);

  // Return 'a || b'.
  template <typename T>
  static T logicalOr(const T& a, const T& b);

  // Return '!a'.
  template <typename T>
  static T logicalNot(const T& a);

  // Return 'a ? b : c'.
  template <typename T>
  static T ifThen(const bool& a, const T& b, const T& c);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
T Operators::logicalNot(const T& a) {
  return !a;
}
template <typename T>
T Operators::logicalAnd(const T& a, const T& b) {
  return a && b;
}
template <typename T>
T Operators::logicalOr(const T& a, const T& b) {
  return a || b;
}
template <typename T>
T Operators::ifThen(const bool& a, const T& b, const T& c) {
  return a ? b : c;
}
template <typename A, typename B>
auto Operators::minus(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a - b)>::type>::type {
  return a - b;
}
template <typename A, typename B>
auto Operators::plus(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a + b)>::type>::type {
  return a + b;
}

template <typename A, typename B>
auto Operators::equalTo(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a == b)>::type>::type {
  return a == b;
}

template <typename A, typename B>
auto Operators::notEqualTo(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a != b)>::type>::type {
  return a != b;
}

template <typename A, typename B>
auto Operators::less(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a < b)>::type>::type {
  return a < b;
}

template <typename A, typename B>
auto Operators::greater(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a > b)>::type>::type {
  return a > b;
}

template <typename A, typename B>
auto Operators::greaterEqual(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a >= b)>::type>::type {
  return a >= b;
}

template <typename A, typename B>
auto Operators::lessEqual(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a <= b)>::type>::type {
  return a <= b;
}

template <typename A, typename B>
auto Operators::multiplies(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a* b)>::type>::type {
  return a * b;
}

template <typename A, typename B>
auto Operators::divides(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a / b)>::type>::type {
  return a / b;
}

template <typename A, typename B>
auto Operators::modulus(const A& a, const B& b)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(a % b)>::type>::type {
  return a % b;
}

template <typename A>
auto Operators::negate(const A& a)
    -> typename boost::remove_reference<
          typename boost::remove_const<decltype(-a)>::type>::type {
  return -a;
}
}

#endif

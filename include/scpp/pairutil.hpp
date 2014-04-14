#ifndef SCPP_UTILITY_HPP_
#define SCPP_UTILITY_HPP_

//@PURPOSE: Provide utility operations for 'std::pair' objects.
//
//@CLASSES:
//  scpp::PairUtil_Swap: pair 'swap' template functor
//  scpp::PairUtil_FusedFunction: pair taking two argument function wrapper
//  scpp::PairUtil: C++ pair utility operation namespace
//
//@DESCRIPTION: This component includes several classes that provide utilities
// that operate on stl pair objects. The 'PairUtil' namespace class includes the
// most commonly used utilities.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Function fusion
// - - - - - - - - - - - - -
// C++ functions that take two arguments can be "fused" into functions that
// take a single pair argument using the 'fuse()' function.
//
// For instance consider the following function,
//..
//  static std::string exampleFunction(int i, std::string s) {
//    return s + std::to_string(i);
//  }
//..
// We can make a fused version of it using the 'fuse' function.
//..
//  auto exampleFunctionFused = scpp::PairUtil::fuse(exampleFunction);
//..
// The fused version can be used just like the original, except that it takes
// in a pair as its argument instead.
//..
//  const std::string result = exampleFunctionFused(std::make_pair(3, "hello"));
//..

#include <utility>  // std::pair

namespace scpp {

// This class provides a template functor implementation of the 'swap()'
// function.
struct PairUtil_Swap {
  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return a pair equivelent to the specified 'pair' object except its first
  // and second elements are switched.
  template <typename A, typename B>
  std::pair<A, B> operator()(const std::pair<B, A>& pair) const;
};

// This class provides a two-argument function wrapper which can be called with
// a 'std::pair' of arguments instead of two individual arguments.
template <typename TwoArgFunction>
struct PairUtil_FusedFunction {
  // Create a new 'PairUtil_FusedFunction' object with the specified
  // 'twoArgFunction'.
  PairUtil_FusedFunction(TwoArgFunction twoArgFunction);

  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return the result of calling the two argument function with the first and
  // second elements of the specified 'pair'.
  template <typename A, typename B>
  typename result<PairUtil_FusedFunction<TwoArgFunction>(std::pair<A, B>)>::type
      operator()(const std::pair<A, B>& pair) const;

private:
  TwoArgFunction m_function;
};

// This class provides utility operations for 'std::pair' objects.
struct PairUtil {
  // Return a 'std::pair' object with the specified 'first' object as its first
  // value and the specified 'second' object as its second value. Note that
  // this function can be used in contexts where a non-overloaded function is
  // required, unlike 'std::make_pair'.
  template <typename First, typename Second>
  static std::pair<First, Second> makePair(const First& first, const Second& second);

  // Return the first element of the specified 'pair'.
  template <typename A, typename B>
  static A first(const std::pair<A, B>& pair);

  // Return the second element of the specified 'pair'.
  template <typename A, typename B>
  static B second(const std::pair<A, B>& pair);

  // Return a version of the specified 'twoArgFunction' that takes in a single
  // 'std::pair' object as a parameter instead of two separate parameters. The
  // behavior is undefined unless 'twoArgFunction' is a function that takes in
  // two arguments.
  template <typename TwoArgFunction>
  static PairUtil_FusedFunction<TwoArgFunction> fuse(
      TwoArgFunction twoArgFunction);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Pair>
struct PairUtil_Swap::result<PairUtil_Swap(Pair)> {
  typedef std::pair<typename Pair::second_type, typename Pair::first_type>
      type;
};

template <typename A, typename B>
std::pair<A, B> PairUtil_Swap::operator()(const std::pair<B, A>& pair) const {
  return std::make_pair(pair.second, pair.first);
}

template <typename TwoArgFunction>
template <typename Pair>
struct PairUtil_FusedFunction<TwoArgFunction>::result<
    PairUtil_FusedFunction<TwoArgFunction>(Pair)> {
  typedef decltype(
      (*(TwoArgFunction*)(0))((*(typename Pair::first_type*)(0)),
                              (*(typename Pair::second_type*)(0)))) type;
};

template <typename TwoArgFunction>
PairUtil_FusedFunction<TwoArgFunction>::PairUtil_FusedFunction(
    TwoArgFunction twoArgFunction)
    : m_function(twoArgFunction) {}

template <typename TwoArgFunction>
template <typename A, typename B>
typename PairUtil_FusedFunction<TwoArgFunction>::result<
    PairUtil_FusedFunction<TwoArgFunction>(std::pair<A, B>)>::type
PairUtil_FusedFunction<TwoArgFunction>::
operator()(const std::pair<A, B>& p) const {
  return m_function(p.first, p.second);
}

template <typename A, typename B>
std::pair<A, B> PairUtil::makePair(const A& a, const B& b) {
  return std::make_pair(a, b);
}

template <typename A, typename B>
A PairUtil::first(const std::pair<A, B>& pm) {
  return pm.first;
}

template <typename A, typename B>
B PairUtil::second(const std::pair<A, B>& pm) {
  return pm.second;
}

template <typename TwoArgFunction>
PairUtil_FusedFunction<TwoArgFunction> PairUtil::fuse(TwoArgFunction twoArgFunction) {
  return PairUtil_FusedFunction<TwoArgFunction>(twoArgFunction);
}

}
#endif

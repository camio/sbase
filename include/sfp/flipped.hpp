#ifndef SFP_FLIPPED_HPP_
#define SFP_FLIPPED_HPP_

//@PURPOSE: Provide a two-argument function wrapper that flips its arguments.
//
//@CLASSES:
//  sfp::Flipped: argument flipping function wrapper
//
//@SEE_ALSO: sfp_fliputil
//
//@DESCRIPTION: This component provides a single class, 'sfp::Flipped', that
// wraps two argument functions into a new function that swaps the argument
// ordering. It works with any sort of two argument callable, including
// polymorphic functors and lambda functions.
//
// This wrapper finds most of its use in higher order functions where the
// caller needs to swap the arguments of the available function. Using
// Boost.Bind as in 'boost::bind( f, _2, _1 )' can handle some of these cases,
// although without polymorphism or lambda support, but the need to flip
// two-argument functions occurs often enough to warrant its own method.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Making a subtract function
// - - - - - - - - - - - - - - - - - - -
// The standard library provides a 'std::minus' functor that subtracts its
// second argument from its first. If we desire a functor where first argument
// subtracted from the second argument, we can use 'sfp::Flipped'.
//..
//  typedef sfp::Flipped<std::minus<int>> Subtract;
//..
// The functor can be used in the usual way.
//..
//  const int one = Subtract()(3, 4);
//..

#include <type_traits>  // std::result_of

namespace sfp {

// This class implements a two-argument function wrapper that flips that
// function's two arguments.
template <typename TwoArgFunction>
struct Flipped {
  // Create a new 'Flipped' object that wraps the specified 'function'.
  Flipped(TwoArgFunction function);

  // Create a new 'Flipped' object that wraps a 'TwoArgFunction' with its empty
  // constructor.
  Flipped();

  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Call the wrapped function with the specified 'secondArg' as its first
  // argument and the specified 'firstArg' as its second argument.
  template <typename FirstArg, typename SecondArg>
  typename result<Flipped<TwoArgFunction>(FirstArg, SecondArg)>::type
  operator()(FirstArg firstArg, SecondArg secondArg) const;
private:
  TwoArgFunction m_function;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename TwoArgFunction>
Flipped<TwoArgFunction>::Flipped(TwoArgFunction function)
    : m_function(function) {}

template <typename TwoArgFunction>
Flipped<TwoArgFunction>::Flipped() {}

template <typename TwoArgFunction>
template <typename A, typename B>
struct Flipped<TwoArgFunction>::result<Flipped<TwoArgFunction>(A, B)> {
  typedef typename std::result_of<TwoArgFunction(B,A)>::type type;
};

template <typename TwoArgFunction>
template <typename FirstArg, typename SecondArg>
typename Flipped<TwoArgFunction>::result<
    Flipped<TwoArgFunction>(FirstArg, SecondArg)>::type
Flipped<TwoArgFunction>::
operator()(FirstArg firstArg, SecondArg secondArg) const {
  return m_function(secondArg, firstArg);
}
}
#endif

#ifndef SFP_FLIPUTIL_HPP_
#define SFP_FLIPUTIL_HPP_

//@PURPOSE: Provide utilities for argument flipping of two argument functions.
//
//@CLASSES:
//  sfp::FlipUtil_Flip: function argument flipping functor
//  sfp::FlipUtil: function argument flipping namespace class
//
//@SEE_ALSO: sfp_flipped
//
//@DESCRIPTION: This component provides utility functions that allow for the
// creation of functions that are a flipped version of another. The functions
// here extend sfp_flipped by removing the need to specify template arguments.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Fitting functions
// - - - - - - - - - - - - - -
// Say we have the following two functions.
//..
//  int foo(A a, B b);
//  int bar(boost::function<int(B, A)> f);
//..
// We'd like to call 'bar' with 'foo' as an argument, but we cannot because the
// arguments of 'foo' are in the wrong order. We use the 'flip()' function to
// make the ordering correct.
//..
//  bar(sfp::FlipUtil::flip(foo));
//..

#include <sfp/flipped.hpp>

namespace sfp {

// This class implements a functor implementation of the 'flip()' function.
struct FlipUtil_Flip {
  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return a version of the specified 'twoArgFunction' that has its arguments
  // flipped. The behavior is undefined unless 'twoArgFunction' has two
  // arguments.
  template <typename TwoArgFunction>
  typename result<FlipUtil_Flip(TwoArgFunction)>::type operator()(
      TwoArgFunction twoArgFunction) const;
};

// This class implements a namespace for functions related to flipping
// arguments.
struct FlipUtil {
  // Return a version of the specified 'twoArgFunction' that has its arguments
  // flipped. The behavior is undefined unless 'twoArgFunction' has two
  // arguments.
  template <typename TwoArgFunction>
  static Flipped<TwoArgFunction> flip(TwoArgFunction twoArgFunction);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename F>
struct FlipUtil_Flip::result<FlipUtil_Flip(F)> {
  typedef Flipped<F> type;
};
template <typename F>
typename FlipUtil_Flip::result<FlipUtil_Flip(F)>::type FlipUtil_Flip::
operator()(F f) const {
  return Flipped<F>(f);
}

template <typename TwoArgFunction>
Flipped<TwoArgFunction> FlipUtil::flip(TwoArgFunction twoArgFunction) {
  return Flipped<TwoArgFunction>(twoArgFunction);
}
}

#endif

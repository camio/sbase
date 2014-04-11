#ifndef SBOOST_FUSION_FOLDUTIL_HPP_
#define SBOOST_FUSION_FOLDUTIL_HPP_

//@PURPOSE: Provide fold operations for Boost.Fusion sequences.
//
//@CLASSES:
//  sboost::FusionFoldUtil_Foldr1Result: foldr1 result type computation
//  sboost::FusionFoldUtil_Foldl1Result: foldl1 result type computation
//  sboost::FusionFoldUtil: Boost.Fusion fold operation namespace
//
//@DESCRIPTION: This component provides fold functions that operate on
// Boost.Fusion sequences. These fold functions extend what is already
// available from the Boost.Fusion library. The generic functions contained
// here are sometimes useful when the need arises to collapse a fusion
// sequence.
//
// The classes 'FusionFoldUtil_Foldr1Result' and 'FusionFoldUtil_Foldl1Result'
// are template metafunctions for computing the result types of
// 'FusionFoldUtil::foldr1' and 'FusionFoldUtil::foldl1'. They serve the same
// purpose as classes in the 'result_of' namespace in the Boost.Fusion library.
//
// The names of 'foldr1()' and 'foldl1()' were chosen to intentionally mimic
// Haskell's versions of these functions.
//
// 'foldr1()' will use a specified function to fold a Boost.Fusion sequence
// from right to left. The '1' indiciates the compile-time requirement that
// this sequence has at least one element.
//
// 'foldl1()' will use a specified function to fold a Boost.Fusion sequence
// from left to right. The '1', again, indiciates the compile-time requirement
// that this sequence has at least one element.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Heterogenius Minimum
// - - - - - - - - - - - - - - - -
// In this example, we'll be constructing a heterogenius 'minimum' function
// that operations on fusion sequences of 'int's, 'float's, or some mixture. We
// start by defining a 'Min' functor that does the same for two elements.
//..
//  struct Min {
//    template <typename T>
//    struct result;
//
//    template <typename T, typename U>
//    struct result<Min(T, U)> {
//      typedef typename boost::remove_const<
//          typename boost::remove_reference<T>::type>::type TValue;
//      typedef typename boost::remove_const<
//          typename boost::remove_reference<U>::type>::type UValue;
//
//      typedef typename boost::mpl::if_<
//          typename boost::mpl::or_<boost::is_same<TValue, float>,
//                                   boost::is_same<UValue, float>>::type,
//          float,
//          int>::type type;
//    };
//
//    int operator()(int i, int j) const { return std::min(i, j); }
//    float operator()(int i, float j) const { return std::min(float(i), j); }
//    float operator()(float i, float j) const { return std::min(i, j); }
//    float operator()(float i, int j) const { return std::min(i, float(j)); }
//  };
//..
// The 'Min::result' type is a template metafunction that computes the result
// type of this functor in the standard way. This format is required for uses
// of the sboost_fusionfoldutil functions (and Boost.Fusion in general).
//
// We now make use of the 'foldl1()' function to write our heterogenius minimum
// function.
//..
//  template <typename Sequence>
//  auto minimum(Sequence s) -> decltype(sboost::FusionFoldUtil::foldl1(s,
// Min())) {
//    return sboost::FusionFoldUtil::foldl1(s, Min());
//  }
//..
// If this function is passed a Boost.Fusion sequence of all 'int's, it will
// return an 'int'. If there is at least one 'float', it will return a float.
// If, on the other hand, the argument is empty, a compilation error will
// occur.

#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/reverse_fold.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_back.hpp>
#include <boost/fusion/include/back.hpp>

namespace sboost {

// Return the type of the result of the 'foldr1' function assuming the
// specified 'Sequence' is the type of its first argument and the specified
// 'Function' is the type of its second argument.
template <typename Sequence, typename F>
struct FusionFoldUtil_Foldr1Result;

// Return the type of the result of the 'foldl1' function assuming the
// specified 'Sequence' is the type of its first argument and the specified
// 'Function' is the type of its second argument.
template <typename Sequence, typename F>
struct FusionFoldUtil_Foldl1Result;

// This class provides a namespace for various fold functions that operate on
// Boost.Fusion sequences.
struct FusionFoldUtil {

  // Recursively call the specified 'binaryFunction' on the spencified
  // 'nonemptySequence' from right to left. If the sequence consists of just one
  // element, return that element. Otherwise return the result of the recursive
  // calls.
  //
  // Generally speaking if the 'nonemptySequence' is 'v₁, v₂, v₃…' and
  // 'binaryFunction' is 'f', this function will return '…f( v₁, f(v₂, v₃) )…'.
  template <typename Sequence, typename F>
  static typename FusionFoldUtil_Foldr1Result<Sequence, F>::type foldr1(
      const Sequence& nonemptySequence,
      F const& binaryFunction);

  // Recursively call the specified 'binaryFunction' on the spencified
  // 'nonemptySequence' from left to right. If the sequence consists of just one
  // element, return that element. Otherwise return the result of the recursive
  // calls.
  //
  // Generally speaking if the 'nonemptySequence' is 'v₁, v₂, v₃…' and
  // 'binaryFunction' is 'f', this function will return '…f( f(v₁, v₂), v₃ )…'.
  template <typename Sequence, typename F>
  static typename FusionFoldUtil_Foldl1Result<Sequence, F>::type foldl1(
      const Sequence& nonemptySequence,
      F const& binaryFunction);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename BinaryFunction>
struct FusionFoldUtil_Flipped {
  // Create a new 'Flipped' object based on the specified 'binaryFunction'
  explicit FusionFoldUtil_Flipped(BinaryFunction binaryFunction)
      : m_binaryFunction(binaryFunction) {}

  typedef FusionFoldUtil_Flipped<BinaryFunction> this_type;

  template <typename T>
  struct result;

  template <typename FirstArg, typename SecondArg>
  struct result<this_type(FirstArg, SecondArg)> {
    typedef typename boost::result_of<BinaryFunction(SecondArg, FirstArg)>::type
        type;
  };

  // Call the binary function this object corresponds to with the specified
  // 'secondArg' as its first argument and 'firstArg' as its second argument.
  template <typename FirstArg, typename SecondArg>
  typename result<this_type(FirstArg, SecondArg)>::type operator()(
      FirstArg firstArg,
      SecondArg secondArg) const {
    return m_binaryFunction(secondArg, firstArg);
  }

 private:
  BinaryFunction m_binaryFunction;
};

template <typename Sequence, typename F>
struct FusionFoldUtil_Foldr1Result {
  typedef typename boost::fusion::result_of::pop_back<Sequence>::type Sequence2;
  typedef typename boost::fusion::result_of::back<Sequence>::type State;

  typedef typename boost::fusion::result_of::
      reverse_fold<Sequence2, State, FusionFoldUtil_Flipped<F>>::type type;
};

template <typename Sequence, typename F>
typename FusionFoldUtil_Foldr1Result<Sequence, F>::type FusionFoldUtil::foldr1(
    const Sequence& seq,
    F const& f) {
  return boost::fusion::reverse_fold(boost::fusion::pop_back(seq),
                                     boost::fusion::back(seq),
                                     FusionFoldUtil_Flipped<F>(f));
}

template <typename Sequence, typename F>
struct FusionFoldUtil_Foldl1Result {
  typedef typename boost::fusion::result_of::pop_front<Sequence>::type
      Sequence2;
  typedef typename boost::fusion::result_of::front<Sequence>::type State;

  typedef typename boost::fusion::result_of::fold<Sequence2, State, F>::type
      type;
};

template <typename Sequence, typename F>
typename FusionFoldUtil_Foldl1Result<Sequence, F>::type FusionFoldUtil::foldl1(
    const Sequence& seq,
    F const& f) {
  return boost::fusion::fold(
      boost::fusion::pop_front(seq), boost::fusion::front(seq), f);
}
}

#endif

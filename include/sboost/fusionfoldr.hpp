#ifndef SBOOST_FUSION_FOLDR1_HPP_
#define SBOOST_FUSION_FOLDR1_HPP_

#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/reverse_fold.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_back.hpp>
#include <boost/fusion/include/back.hpp>

namespace sboost {

namespace result_of {

// Return the type of the result of the 'foldr1' function assuming the
// specified 'Sequence' is the type of its first argument and the specified
// 'Function' is the type of its second argument.
template <typename Sequence, typename F>
struct foldr1;

// Return the type of the result of the 'foldl1' function assuming the
// specified 'Sequence' is the type of its first argument and the specified
// 'Function' is the type of its second argument.
template <typename Sequence, typename F>
struct foldl1;
}

// Recursively call the specified 'binaryFunction' on the spencified
// 'nonemptySequence' from right to left. If the sequence consists of just one
// element, return that element. Otherwise return the result of the recursive
// calls.
//
// Generally speaking if the 'nonemptySequence' is 'v₁, v₂, v₃…' and
// 'binaryFunction' is 'f', this function will return '…f( v₁, f(v₂, v₃) )…'.
template <typename Sequence, typename F>
typename result_of::foldr1<Sequence, F>::type foldr1(Sequence& nonemptySequence,
                                                     F const& binaryFunction);

// Recursively call the specified 'binaryFunction' on the spencified
// 'nonemptySequence' from left to right. If the sequence consists of just one
// element, return that element. Otherwise return the result of the recursive
// calls.
//
// Generally speaking if the 'nonemptySequence' is 'v₁, v₂, v₃…' and
// 'binaryFunction' is 'f', this function will return '…f( f(v₁, v₂), v₃ )…'.
template <typename Sequence, typename F>
typename result_of::foldl1<Sequence, F>::type foldl1(Sequence& seq,
                                                     F const& f);

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

namespace detail {
template <typename BinaryFunction>
struct Flipped {
  // Create a new 'Flipped' object based on the specified 'binaryFunction'
  explicit Flipped(BinaryFunction binaryFunction)
      : m_binaryFunction(binaryFunction) {}

  typedef Flipped<BinaryFunction> this_type;

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
}

namespace result_of {
template <typename Sequence, typename F>
struct foldr1 {
  typedef typename boost::fusion::result_of::pop_back<Sequence const>::type
      Sequence2;
  typedef typename boost::fusion::result_of::back<Sequence>::type State;

  typedef typename boost::fusion::result_of::
      reverse_fold<Sequence2, State, detail::Flipped<F>>::type type;
};
}


template <typename Sequence, typename F>
typename result_of::foldr1<Sequence, F>::type foldr1(Sequence& seq,
                                                     F const& f) {
  return boost::fusion::reverse_fold(boost::fusion::pop_back(seq),
                                     boost::fusion::back(seq),
                                     detail::Flipped<F>(f));
}

namespace result_of {
template <typename Sequence, typename F>
struct foldl1 {
  typedef typename boost::fusion::result_of::pop_front<Sequence>::type
      Sequence2;
  typedef typename boost::fusion::result_of::front<Sequence>::type State;

  typedef typename boost::fusion::result_of::fold<Sequence2, State, F>::type
      type;
};
}

template <typename Sequence, typename F>
typename result_of::foldl1<Sequence, F>::type foldl1(Sequence& seq,
                                                     F const& f) {
  return boost::fusion::fold(
      boost::fusion::pop_front(seq), boost::fusion::front(seq), f);
}
}

#endif

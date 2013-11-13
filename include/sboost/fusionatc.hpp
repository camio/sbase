#ifndef SBOOST_FUSIONATC_HPP_
#define SBOOST_FUSIONATC_HPP_

//@PURPOSE: Provide a 'fusion::at_c' that is usable in higher order functions.
//
//@CLASSES:
//  sboost::FusionAtC: N-th element of fusion sequence functor.
//
//@DESCRIPTION: This component provides a single class, 'FusionAtC', that can
// be used much like the 'boost::fusion::at_c' function except it can also be
// used in higher order functions once 'N' is known.

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/value_at.hpp>

namespace sboost {

// Implements a functor that returns the N-th element from the beginning of a
// fusion sequence.
template <int N>
struct FusionAtC {
  typedef FusionAtC<N> this_type;

  template <typename T>
  struct result;

  template <typename S>
  struct result<this_type(const S)> {
    typedef typename boost::fusion::result_of::value_at_c<S, N>::type type;
  };

  // Return the N-th element from the beginning of a fusion sequence.
  template <typename S>
  typename result<this_type(const S)>::type operator()(const S& s) const {
    return boost::fusion::at_c<N>(s);
  }
};
}

#endif

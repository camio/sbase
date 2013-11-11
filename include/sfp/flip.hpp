#ifndef SFP_FLIP_HPP_
#define SFP_FLIP_HPP_

#include <boost/utility/result_of.hpp>

namespace sfp {
namespace detail {
template <typename F>
struct Flip1 {
  typedef Flip1<F> this_type;

  explicit Flip1(F f_) : f(f_) {}
  F f;
  template <typename T>
  struct result;

  template <typename A, typename B>
  struct result<this_type(A, B)> {
    typedef typename boost::result_of<F(B, A)>::type type;
  };

  template <typename A, typename B>
  typename result<this_type(A, B)>::type operator()(A a, B b) const {
    return f(b, a);
  }
};
}

struct Flip {
  typedef Flip this_type;

  template <typename T>
  struct result;

  template <typename F>
  struct result<this_type(F)> {
    typedef detail::Flip1<F> type;
  };

  template <typename F>
  typename result<this_type(F)>::type operator()(F f) const {
    return detail::Flip1<F>(f);
  }
};

const Flip flip;
}

#endif

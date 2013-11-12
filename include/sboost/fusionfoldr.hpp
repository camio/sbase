#ifndef SBOOST_FUSION_FOLDR1_HPP_
#define SBOOST_FUSION_FOLDR1_HPP_

#include <boost/fusion/include/fold.hpp>
#include <boost/fusion/include/reverse_fold.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_back.hpp>
#include <boost/fusion/include/back.hpp>

namespace sboost {
namespace fusion {
namespace result_of {
template <typename Sequence, typename F>
struct foldr1 {
  typedef typename boost::fusion::result_of::pop_front<Sequence const>::type
      Sequence2;
  typedef typename boost::fusion::result_of::front<Sequence>::type State;

  typedef typename boost::fusion::result_of::fold<Sequence2, State, F>::type
      type;
};
}

template <typename Sequence, typename F>
typename result_of::foldr1<Sequence, F>::type foldr1(Sequence& seq,
                                                     F const& f) {
  using namespace boost::fusion;
  return fold(pop_front(seq), front(seq), f);
}
}
namespace fusion {
namespace result_of {
template <typename Sequence, typename F>
struct foldl1 {
  typedef typename boost::fusion::result_of::pop_back<Sequence const>::type
      Sequence2;
  typedef typename boost::fusion::result_of::back<Sequence>::type State;

  typedef typename boost::fusion::result_of::reverse_fold<Sequence2,
                                                          State,
                                                          F>::type type;
};
}

template <typename Sequence, typename F>
typename result_of::foldl1<Sequence, F>::type foldl1(Sequence& seq,
                                                     F const& f) {
  using namespace boost::fusion;
  return reverse_fold(pop_back(seq), back(seq), f);
}
}
}

#endif

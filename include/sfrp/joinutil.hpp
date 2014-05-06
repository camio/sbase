#ifndef SFRP_JOINUTIL_HPP_
#define SFRP_JOINUTIL_HPP_

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/joinpullfunc.hpp>

namespace sfrp {
struct JoinUtil {

  // Flatten a specially typed and specially valued Behavior. Return the result.
  // Thebehavior is undefined unless whenever 'behavior' has not 'boost::none',
  // the value must be in 0 state.  That is, calling pull with a time of 0
  // should
  // be valid.
  //..
  //  pmJoin : Behavior (Maybe (Behavior a)) → Behavior (Maybe a)
  //..
  template <typename A>
  static sfrp::Behavior<boost::optional<A>> join(
      const sfrp::Behavior<boost::optional<sfrp::Behavior<A>>>& behavior);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A>
static sfrp::Behavior<boost::optional<A>> JoinUtil::join(
    const sfrp::Behavior<boost::optional<sfrp::Behavior<A>>>& behavior) {
  const boost::shared_ptr<JoinPullFunc<A>> joinPullFuncPtr =
      boost::make_shared<JoinPullFunc<A>>(behavior);

  return sfrp::Behavior<boost::optional<A>>::fromValuePullFunc(
      boost::bind(&JoinPullFunc<A>::pullVal, joinPullFuncPtr, _1));
}
}
#endif

#ifndef SFRP_JOINPULLFUNC_HPP_
#define SFRP_JOINPULLFUNC_HPP_

#include <boost/optional.hpp>
#include <sfrp/behavior.hpp>
#include <utility>  // std::pair

namespace sfrp {
template <typename A>
class JoinPullFunc {
  typedef boost::optional<boost::optional<A>> result_type;

 public:
  JoinPullFunc(const sfrp::Behavior<boost::optional<sfrp::Behavior<A>>>&
                   joinableBehavior);

  result_type pullVal(const double time);

 private:
  boost::optional<std::pair<double,  // dTime
                            sfrp::Behavior<A>>> m_state;
  sfrp::Behavior<boost::optional<sfrp::Behavior<A>>> m_joinableBehavior;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A>
JoinPullFunc<A>::JoinPullFunc(
    const sfrp::Behavior<boost::optional<sfrp::Behavior<A>>>& joinableBehavior)
    : m_joinableBehavior(joinableBehavior), m_state(boost::none) {}

template <typename A>
typename JoinPullFunc<A>::result_type JoinPullFunc<A>::pullVal(
    const double time) {
  const boost::optional<boost::optional<sfrp::Behavior<A>>> opOpBehaviorA =
      m_joinableBehavior.pull(time);
  if (opOpBehaviorA) {
    const boost::optional<sfrp::Behavior<A>>& opBehaviorA = *opOpBehaviorA;
    if (opBehaviorA) {
      m_state = boost::make_optional(std::make_pair(time, *opBehaviorA));
      const boost::optional<A> opA = m_state->second.pull(0);
      if (!opA)
        m_state = boost::none;
      return boost::make_optional(opA);
    } else {
      if (m_state) {
        const boost::optional<A> opA =
            m_state->second.pull(time - m_state->first);
        if (opA)
          return boost::make_optional(opA);
        else {
          m_state = boost::none;
          return result_type(boost::optional<A>());
        }
      } else {
        return result_type(boost::optional<A>());
      }
    }
  } else {
    if (m_state) {
      const boost::optional<A> opA =
          m_state->second.pull(time - m_state->first);
      if (opA)
        return boost::make_optional(opA);
      else {
        m_state = boost::none;
        return boost::none;
      }
    } else
      return boost::none;
  }
}
}
#endif

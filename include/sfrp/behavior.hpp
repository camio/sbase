#ifndef SFRP_BEHAVIOR_HPP_
#define SFRP_BEHAVIOR_HPP_

#include <boost/function.hpp>
#include <boost/optional.hpp>

namespace sfrp {

/** See "Interactive Move Semantics.txt" for a meaning
 */
template <typename A>
struct Behavior {
  typedef A type;

  explicit Behavior(boost::function<boost::optional<A>(double)> valuePullFunc);

  // Create 'Behavior' object that has no value at any time.
  Behavior();

  // Create 'Behavior' object equivelent to the specified 'behavior'.
  Behavior( Behavior<A> && behavior );

  boost::optional<A> pull(const double time) const;

 private:
  boost::function<boost::optional<A>(double)> m_valuePullFunc;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename A>
Behavior<A>::Behavior(boost::function<boost::optional<A>(double)> valuePullFunc)
    : m_valuePullFunc(std::move(valuePullFunc)) {}

template <typename A>
Behavior<A>::Behavior()
    : m_valuePullFunc([](double)->boost::optional<A>{
  return boost::none;
}) {}

template <typename A>
boost::optional<A> Behavior<A>::pull(const double time) const {
  return m_valuePullFunc(time);
}

template <typename A>
Behavior<A>::Behavior(Behavior<A>&& behavior)
    : m_valuePullFunc(std::move(behavior.m_valuePullFunc)) {}
}
#endif

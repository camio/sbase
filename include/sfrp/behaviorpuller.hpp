#ifndef SFRP_BEHAVIORPULLER_HPP_
#define SFRP_BEHAVIORPULLER_HPP_

//@PURPOSE: Provide a functor that pulls behaviors at a particular time.
//
//@CLASSES:
//  sfrp::BehaviorPuller: specific time behavior pull functor
//
//@DESCRIPTION: This component provides a single functor class,
// 'BehaviorPuller', that can be used to pull behaviors at a particular time. It
// is, in essence, a partial application of the 'pull()' function of 'Behavior'
// objects.
//
// The intended use of this object is in higher order functions such as behavior
// mapping.
//
// For a partial application as simple as this, one might think to use other
// means to construct this kind of functor. A C++11 anonymous function doesn't
// have the requisite polymorphism support, Boost.Bind doesn't support
// polymorphic result types, and using dbb or phoenix would introduce an
// unwanted dependency. Given this case, the 'BehaviorPuller' object has its
// use.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Pull at time '0'
// - - - - - - - - - - - - - -
// First, we set up a couple behaviors we will be pulling from.
//..
//  // Set 'intTime' to the behavior that is the integer value of whatever time
//  // it is.
//  sfrp::Behavior<int> intTime = sfrp::Behavior<int>::fromValuePullFunc([](
//      double time) { return int(time); });
//
//  // Set 'alwaysFour' to a behavior that is "Four" for all times.
//  sfrp::Behavior<std::string> alwaysFour =
//      sfrp::Behavior<std::string>::fromValuePullFunc([](double time) {
//        return std::string("Four");
//      });
//..
// Now we create our behavior puller and use it to pull values at time '1.0'.
//..
//  sfrp::BehaviorPuller atOne(1.0);
//  // Will be 'make_optional(1)'
//  boost::optional<int> intTimeReult = atOne( intTime );
//  // Will be 'make_optional(string("Four"))'
//  boost::optional<std::string> alwaysFourResult = atOne( alwaysFour );
//..

#include <boost/optional.hpp>
#include <sfrp/behavior.hpp>
#include <type_traits>  // std::remove_const, std::remove_reference

namespace sfrp {

// This class implements a partial application of the 'pull()' method of
// behaviors where the time is set beforehand.
struct BehaviorPuller {
  // Create a new 'BehaviorPuller' object that pulls behaviors at the specified
  // 'time'.
  BehaviorPuller(double time);

  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return the result of calling the 'pull()' function of the specified
  // 'behavior' with a time argument corresponding to the time of this
  // 'BehaviorPuller'.
  template <typename Behavior>
  typename result<BehaviorPuller(const Behavior&)>::type operator()(
      const Behavior&) const;

 private:
  double m_time;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename BehaviorArg>
struct BehaviorPuller::result<BehaviorPuller(BehaviorArg)> {
  typedef typename typename std::remove_reference<
      typename std::remove_const<BehaviorArg>::type>::type::type BehaviorValue;
  typedef boost::optional<BehaviorValue> type;
};

template <typename Behavior>
typename BehaviorPuller::result<BehaviorPuller(const Behavior&)>::type
BehaviorPuller::
operator()(const Behavior& b) const {
  return b.pull(m_time);
}
}
#endif

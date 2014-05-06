#ifndef SFRP_TRIGGERUTIL_HPP_
#define SFRP_TRIGGERUTIL_HPP_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/eventutil.hpp>
#include <sfrp/triggerimpl.hpp>

namespace sfrp {
struct TriggerUtil {
  // Create an event-like behavior that occurs whenever the returned function is
  // called. If the returned function is called with boost::none, it signifies
  // the end of the behavior. The function is thread-safe.
  template <typename T>
  static std::pair<Behavior<boost::optional<T>>,
                   boost::function<void(const boost::optional<T>&)>>
      trigger();

  // Create a never-ending event-like behavior that occurs whenever the returned
  // function is called. The resulting function is thread safe.
  template <typename T>
  static std::pair<Behavior<boost::optional<T>>,
                   boost::function<void(const T&)>>
      triggerInf();

  // Create a never-ending behavior whose value is initial the specified 't0'
  // and will change based on calls to the resulting function.  The resulting
  // function is thread safe.
  template <typename T>
  static std::pair<Behavior<T>, boost::function<void(const T&)>> triggerInfStep(
      const T& t0);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
std::pair<Behavior<boost::optional<T>>,
          boost::function<void(const boost::optional<T>&)>>
TriggerUtil::trigger() {
  const auto triggerImpPtr = boost::make_shared<TriggerImpl<T>>();
  return std::make_pair(
      Behavior<boost::optional<T>>::fromValuePullFunc(
          boost::bind(&TriggerImpl<T>::pullVal, triggerImpPtr, _1)),
      boost::bind(&TriggerImpl<T>::loadVal, triggerImpPtr, _1));
}

template <typename T>
std::pair<Behavior<boost::optional<T>>, boost::function<void(const T&)>>
TriggerUtil::triggerInf() {
  auto pairPmF = TriggerUtil::trigger<T>();
  return std::make_pair(pairPmF.first, [pairPmF](const T & t) {
    return pairPmF.second(boost::make_optional(t));
  });
}

template <typename T>
std::pair<Behavior<T>, boost::function<void(const T&)>>
TriggerUtil::triggerInfStep(const T& t0) {
  auto pairPmF = TriggerUtil::triggerInf<T>();
  return std::make_pair(sfrp::EventUtil::step(t0, pairPmF.first),
                        pairPmF.second);
}
}
#endif

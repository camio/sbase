#ifndef SFRP_WORMHOLE_HPP_
#define SFRP_WORMHOLE_HPP_

//@SEE_ALSO: sfrp_wormholeutil

#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/behavior.hpp>
#include <utility>  // std::pair

namespace sfrp {

template <typename T>
struct Wormhole {
  typedef T type;

  // Create a new wormhole object that has an initial output of the specified
  // 'value'. The newly created wormhole is not closed.
  Wormhole(const T& value);

  // Set this wormhole's input to the specified 'inputBehavior'. Return a
  // behavior equivelent to 'inputBehavior', but which feeds this wormhole as a
  // side-effect of 'pull' operations. This wormhole is then set to closed. The
  // behavior is undefined unless 'closed() == false'.
  sfrp::Behavior<T> setInputBehavior(const Behavior<T>& inputBehavior) const;

  // Return 'true' if this wormhole is closed and 'false' otherwise.
  bool closed() const;

  // Return the output behavior corresonding to this wormhole.
  sfrp::Behavior<T> outputBehavior() const;

 private:
  template <typename T>
  friend struct Wormhole_ClosedBehaviorFunction;

  Wormhole(const boost::shared_ptr<std::pair<bool, boost::optional<T>>>& data_);

  boost::shared_ptr<std::pair<bool, boost::optional<T>>> m_data;

  sfrp::Behavior<T> m_outputBehavior;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Wormhole<T>::Wormhole(
    const boost::shared_ptr<std::pair<bool, boost::optional<T>>>& data_)
    : m_data(data_) {}

template <typename T>
bool Wormhole<T>::closed() const {
  return m_data->first;
}

template <typename T>
sfrp::Behavior<T> Wormhole<T>::outputBehavior() const
{
  return m_outputBehavior;
}

template <typename T>
struct Wormhole_BehaviorFunction {
  Wormhole_BehaviorFunction(
      const boost::shared_ptr<std::pair<bool, boost::optional<T>>>& data_)
      : data(data_) {}
  typedef boost::optional<T> result_type;
  result_type operator()(const double time) const { return data->second; }
  boost::shared_ptr<std::pair<bool, boost::optional<T>>> data;
};

template <typename T>
Wormhole<T>::Wormhole(const T& value)
    : m_data(boost::make_shared<std::pair<bool, boost::optional<T>>>(false,
                                                                     value)),
      m_outputBehavior(Behavior<T>(Wormhole_BehaviorFunction<T>(m_data))) {}

template <typename T>
struct Wormhole_ClosedBehaviorFunction {
  Wormhole_ClosedBehaviorFunction(const Wormhole<T>& wh_,
                                  const Behavior<T>& pm_)
      : wh(wh_), pm(pm_) {}
  typedef boost::optional<T> result_type;
  result_type operator()(const double time) const {
    return wh.m_data->second = pm.pull(time);
  }
  Wormhole<T> wh;
  Behavior<T> pm;
};

template <typename T>
Behavior<T> Wormhole<T>::setInputBehavior(const Behavior<T>& pm) const {
  m_data->first = true;
  return Behavior<T>(Wormhole_ClosedBehaviorFunction<T>(*this, pm));
}
}
#endif

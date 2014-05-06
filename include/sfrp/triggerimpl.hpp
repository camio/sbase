#ifndef SFRP_TRIGGERIMPL_HPP_
#define SFRP_TRIGGERIMPL_HPP_

#include <boost/optional.hpp>
#include <mutex>

namespace sfrp {

template <typename T>
struct TriggerImpl {

  boost::optional<boost::optional<T>> pullVal(const double time);

  void loadVal(const boost::optional<T>& opT);

 private:
  std::mutex m_mutex;
  boost::optional       // is there anything loaded?
      <boost::optional  // Is the thing loaded an "exit" or a
       <T               // ← value ?
        >> m_state;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
void TriggerImpl<T>::loadVal(const boost::optional<T>& opT) {
  m_mutex.lock();
  if (!(m_state && !(*m_state)))
    m_state = boost::optional<boost::optional<T>>(opT);
  m_mutex.unlock();
}
template <typename T>
boost::optional<boost::optional<T>> TriggerImpl<T>::pullVal(const double time) {
  m_mutex.lock();

  const boost::optional<boost::optional<T>> ret =
      m_state ? (*m_state ? boost::optional<boost::optional<T>>(
                                boost::optional<T>(**m_state))
                          : boost::optional<boost::optional<T>>())
              : boost::optional<boost::optional<T>>(boost::optional<T>());
  if (ret)
    m_state = boost::optional<boost::optional<T>>();
  m_mutex.unlock();
  return ret;
}
}
#endif

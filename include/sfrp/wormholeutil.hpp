#ifndef SFRP_WORMHOLEUTIL_HPP_
#define SFRP_WORMHOLEUTIL_HPP_

#include <sfrp/behavior.hpp>
#include <sfrp/wormhole.hpp>
#include <utility>  // std::pair

namespace sfrp {

struct WormholeUtil {
  template <typename T>
  static Wormhole<T> createWormhole(const T& t0);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
static Wormhole<T> WormholeUtil::createWormhole(const T& t0) {
  return sfrp::Wormhole<T>(t0);
}
}
#endif

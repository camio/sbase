#ifndef SFRP_WORMHOLEUTIL_HPP_
#define SFRP_WORMHOLEUTIL_HPP_

//@PURPOSE: Provide utility operations for 'Wormhole' objects.
//
//@CLASSES:
//  sfrp::WormholeUtil: wormhole utility operation namespace
//
//@SEE_ALSO: sfrp_wormhole
//
//@DESCRIPTION: This component provides a single namespace class,
// 'WormholeUtil', which includes utility operations for 'Wormhole' objects.
//
// The single function in this namespace is 'createWormhole' which is a
// shorthand for the 'Wormhole' constructor because a type need not be
// specified since it is deduced from the argument.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Create a 'double' wormhole
// - - - - - - - - - - - - - - - - - - -
// In this example we create a 'double' wormhole with an initial value of '0.0'.
//..
//  auto doubleWormhole = sfrp::WormholeUtil::createWormhole( 0.0 );
//..
// Note that the fact that this wormhole has type double is deduced based on the
// argument of the 'createWormhole()' function.

#include <sfrp/wormhole.hpp>

namespace sfrp {

// This class is a namespace for utility operations on 'Wormhole' objects.
struct WormholeUtil {
  // Create a new 'Wormhole' object with the specified 'initialValue' as its
  // initial value.
  template <typename T>
  static Wormhole<T> createWormhole(const T& initialValue);
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

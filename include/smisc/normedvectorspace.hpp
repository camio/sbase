#ifndef SMISC_NORMEDVECTORSPACE_HPP_
#define SMISC_NORMEDVECTORSPACE_HPP_

#include <smisc/point1d.hpp>

namespace smisc {
template <typename T>
struct NormedVectorSpace;

template <typename T>
Point1D norm(const T& t);

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
Point1D norm(const T& t) {
  return NormedVectorSpace<T>().norm_(t);
}
}

#endif

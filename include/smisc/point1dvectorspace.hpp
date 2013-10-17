#ifndef SMISC_POINT1DVECTORSPACE_HPP_
#define SMISC_POINT1DVECTORSPACE_HPP_

#include <smisc/point1d.hpp>
#include <smisc/vectorspace.hpp>

namespace smisc {

template <>
struct VectorSpace<Point1D> {
  Point1D zero_() const { return 0.0; }
};
}
#endif

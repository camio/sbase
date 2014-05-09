#include <smisc/point2dvectorspace.hpp>

#include <smisc/point1dvectorspace.hpp>

namespace smisc {
smisc::Point2D VectorSpace<smisc::Point2D>::zero_() const {
  return smisc::Point2D(zero<smisc::Point1D>(), zero<smisc::Point1D>());
}
}

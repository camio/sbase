#include <smisc/point2dnormedvectorspace.hpp>

#include <cmath> // std::sqrt

namespace smisc {
smisc::Point1D NormedVectorSpace<smisc::Point2D>::norm_(
    const smisc::Point2D& p) const {
  return std::sqrt(p.x * p.x + p.y * p.y);
}
}

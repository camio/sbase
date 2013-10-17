#include <smisc/normedvectorspace.hpp>
#include <smisc/point1d.hpp>
#include <smisc/point1dvectorspace.hpp>
#include <cmath>

namespace smisc {
template <>
struct NormedVectorSpace<Point1D> {
  Point1D norm_(const Point1D d) const { return std::fabs(d); }
};
}

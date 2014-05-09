#ifndef SMISC_POINT2DNORMEDVECTORSPACE_HPP_
#define SMISC_POINT2DNORMEDVECTORSPACE_HPP_

#include <smisc/point2d.hpp>
#include <smisc/point2dvectorspace.hpp>
#include <smisc/normedvectorspace.hpp>

namespace smisc {
template <>
struct NormedVectorSpace<smisc::Point2D> {
  smisc::Point1D norm_(const smisc::Point2D& point) const;
};
}

#endif

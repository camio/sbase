#ifndef SMISC_POINT2DVECTORSPACE_HPP_
#define SMISC_POINT2DVECTORSPACE_HPP_

#include <smisc/point2d.hpp>
#include <smisc/vectorspace.hpp>

namespace smisc {
template <>
struct VectorSpace<smisc::Point2D> {
  smisc::Point2D zero_() const;
};
}
#endif

#ifndef SMISC_POINT2D_HPP_
#define SMISC_POINT2D_HPP_

#include <smisc/point1d.hpp>

namespace smisc {
struct Point2D {
  // Create a new 'Point2D' object with unspecified coordinates.
  Point2D();
  // Create a new 'Point2D' object with the specified 'x' and 'y' coordinates.
  Point2D(const smisc::Point1D x, const smisc::Point1D y);
  Point2D& operator+=(const Point2D& b);
  smisc::Point1D x;
  smisc::Point1D y;
};
Point2D operator-(const Point2D& a, const Point2D& b);
Point2D operator+(const Point2D& a, const Point2D& b);
Point2D operator*(const smisc::Point1D t, const Point2D& b);
Point2D operator*(const Point2D& b, const smisc::Point1D t);
Point2D operator/(const Point2D a, const smisc::Point1D& t);
smisc::Point1D p2X(const Point2D& p);
smisc::Point1D p2Y(const Point2D& p);
}

#endif

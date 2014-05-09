#include <smisc/point2d.hpp>

namespace smisc {
Point2D::Point2D() {}
Point2D::Point2D(const smisc::Point1D x_, const smisc::Point1D y_)
    : x(x_), y(y_) {}
Point2D& Point2D::operator+=(const Point2D& b) {
  x += b.x;
  y += b.y;
  return *this;
}
Point2D operator-(const Point2D& a, const Point2D& b) {
  return Point2D(a.x - b.x, a.y - b.y);
}
Point2D operator+(const Point2D& a, const Point2D& b) {
  return Point2D(a.x + b.x, a.y + b.y);
}
Point2D operator*(const smisc::Point1D t, const Point2D& b) {
  return Point2D(t * b.x, t * b.y);
}
Point2D operator*(const Point2D& b, const smisc::Point1D t) {
  return Point2D(t * b.x, t * b.y);
}
Point2D operator/(const Point2D a, const smisc::Point1D& t) {
  return Point2D(a.x / t, a.y / t);
}
smisc::Point1D p2X(const Point2D& p) { return p.x; }
smisc::Point1D p2Y(const Point2D& p) { return p.y; }
}

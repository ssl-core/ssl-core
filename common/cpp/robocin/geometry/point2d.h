#ifndef ROBOCIN_GEOMETRY_POINT2D_H
#define ROBOCIN_GEOMETRY_POINT2D_H

#include <math.h>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace robocin {

template <class T>
struct Point2D {
  using value_type = T;

  value_type x, y;

  // Constructors:
  inline constexpr Point2D() : x(0), y(0) {}
  inline constexpr Point2D(value_type x, value_type y) : x(x), y(y) {}

  template <class U, class V>
  inline constexpr Point2D(const std::pair<U, V>& pair) :
      x(static_cast<value_type>(pair.first)),
      y(static_cast<value_type>(pair.second)) {}

  // Static methods:
  inline static constexpr Point2D null() { return Point2D(0, 0); }

  inline static constexpr Point2D fromPolar(value_type angle) {
    return Point2D(std::cos(angle), std::sin(angle));
  }

  inline static constexpr Point2D fromPolar(value_type length, value_type angle) {
    return Point2D(std::cos(angle) * length, std::sin(angle) * length);
  }

  // Floatint-point comparison:
  bool fuzzyIsNull(value_type value, value_type epsilon = static_cast<value_type>(1e-9)) {
    return std::abs(value) < epsilon;
  }

  inline constexpr bool operator==(const Point2D& other) const {
    return fuzzyIsNull(other.x - x) and fuzzyIsNull(other.y - y);
  }

  // Arithmetic-assignment operators:
  inline constexpr Point2D& operator+=(const Point2D& other) {
    x += other.x, y += other.y;
    return *this;
  }

  inline constexpr Point2D& operator-=(const Point2D& other) {
    x -= other.x, y -= other.y;
    return *this;
  }

  inline constexpr Point2D& operator*=(const value_type factor) {
    x *= factor, y *= factor;
    return *this;
  }

  inline constexpr Point2D& operator/=(const value_type factor) {
    x /= factor, y /= factor;
    return *this;
  }

  // arithmetic operators:
  inline constexpr Point2D operator+(const Point2D& other) const { return Point2D(*this) += other; }

  inline constexpr Point2D operator-(const Point2D& other) const { return Point2D(*this) -= other; }

  inline constexpr Point2D operator*(value_type factor) const { return Point2D(*this) *= factor; }

  inline constexpr Point2D operator/(value_type factor) const { return Point2D(*this) /= factor; }

  // arithmetic friends:
  friend inline constexpr Point2D operator*(value_type factor, const Point2D& point) {
    return Point2D(factor * point.x, factor * point.y);
  }

  // sign operators:
  inline constexpr Point2D operator+() const { return *this; }

  inline constexpr Point2D operator-() const { return Point2D(-x, -y); }

  // Custom
  [[nodiscard]] inline constexpr bool isNull() const { return fuzzyIsNull(x) and fuzzyIsNull(y); }

  inline constexpr void transpose() { std::swap(x, y); }

  inline constexpr Point2D transposed() const { return Point2D{y, x}; }

  inline constexpr void swap(Point2D& other) noexcept {
    std::swap(x, other.x);
    std::swap(y, other.y);
  }

  inline constexpr std::pair<value_type, value_type> toPair() const { return std::make_pair(x, y); }
  template <class U, class V = U>
  inline constexpr std::pair<U, V> toPair() const {
    return std::make_pair(static_cast<U>(x), static_cast<V>(y));
  }

  inline constexpr value_type manhattanLength() const { return std::abs(x) + std::abs(y); }

  inline constexpr value_type dot(const Point2D& other) const { return x * other.x + y * other.y; }

  inline constexpr value_type cross(const Point2D& other) const {
    return x * other.y - y * other.x;
  }

  inline constexpr value_type distanceSquaredTo(const Point2D& other) const {
    return (other.x - x) * (other.x - x) + (other.y - y) * (other.y - y);
  }

  inline constexpr auto distanceTo(const Point2D& other) const {
    return std::sqrt(distanceSquaredTo(other));
  }

  inline constexpr void rotateClockWise90() { std::swap(x, y), y = -y; }

  inline constexpr Point2D rotatedClockWise90() const {
    Point2D result{*this};
    result.rotateClockWise90();
    return result;
  }

  inline constexpr void rotateCounterClockWise90() { std::swap(x, y), x = -x; }

  inline constexpr Point2D rotatedCounterClockWise90() const {
    Point2D result{*this};
    result.rotateCounterClockWise90();
    return result;
  }

  inline constexpr void rotateClockWise(value_type t) {
    const auto cos = std::cos(t);
    const auto sin = std::sin(t);

    const auto rx = x * cos + y * sin;
    const auto ry = -x * sin + y * cos;

    x = rx, y = ry;
  }

  inline constexpr Point2D rotatedClockWise(value_type t) const {
    Point2D result{*this};
    result.rotateClockWise(t);
    return result;
  }

  inline constexpr void rotateCounterClockWise(value_type t) {
    const auto cos = std::cos(t);
    const auto sin = std::sin(t);

    const auto rx = x * cos - y * sin;
    const auto ry = x * sin + y * cos;

    x = rx, y = ry;
  }

  inline constexpr Point2D rotatedCounterClockWise(value_type t) const {
    Point2D result{*this};
    result.rotateCounterClockWise(t);
    return result;
  }

  inline constexpr auto angle() const { return std::atan2(y, x); }

  inline constexpr auto angleTo(const Point2D& other) const {
    return std::atan2(cross(other), dot(other));
  }

  inline constexpr value_type lengthSquared() const { return dot(*this); }

  inline constexpr auto length() const { return std::sqrt(lengthSquared()); }

  inline constexpr auto norm() const { return std::sqrt(lengthSquared()); }

  inline constexpr void resize(value_type t) {
    if (auto norm = this->norm(); not fuzzyIsNull(norm)) {
      x *= t / norm, y *= t / norm;
    }
  }

  inline constexpr Point2D resized(value_type t) const {
    Point2D result{*this};
    result.resize(t);
    return result;
  }

  inline constexpr void normalize() {
    if (auto norm = this->norm(); not fuzzyIsNull(norm)) {
      x /= norm, y /= norm;
    }
  }

  inline constexpr Point2D normalized() const {
    Point2D result{*this};
    result.normalize();
    return result;
  }

  inline constexpr void projectOntoLine(const Point2D& a, const Point2D& b) {
    if (a == b) {
      throw std::invalid_argument("projectOntoLine: a == b");
    }
    const Point2D ab = b - a;
    *this = a + ab * ab.dot(*this - a) / ab.dot(ab);
  }

  inline constexpr Point2D projectedOntoLine(const Point2D& a, const Point2D& b) const {
    Point2D result{*this};
    result.projectOntoLine(a, b);
    return result;
  }

  inline constexpr void projectOntoSegment(const Point2D& a, const Point2D& b) {
    if (a == b) {
      throw std::invalid_argument("projectOntoSegment: a == b");
    }
    const auto ab = b - a;
    auto r = ab.dot(ab);
    if (fuzzyIsNull(r)) {
      *this = a;
    } else {
      r = (*this - a).dot(ab) / r;
      if (r < 0) {
        *this = a;
      } else if (r > 1) {
        *this = b;
      } else {
        *this = a + ab * r;
      }
    }
  }

  inline constexpr Point2D projectedOntoSegment(const Point2D& a, const Point2D& b) const {
    Point2D result{*this};
    result.projectOntoSegment(a, b);
    return result;
  }

  inline constexpr void reflectOntoLine(const Point2D& a, const Point2D& b) {
    if (a == b) {
      throw std::invalid_argument("reflectOntoLine: a == b");
    }
    *this = static_cast<value_type>(2) * projectedOntoLine(a, b) - *this;
  }

  inline constexpr Point2D reflectedOntoLine(const Point2D& a, const Point2D& b) const {
    Point2D result{*this};
    result.reflectOntoLine(a, b);
    return result;
  }

  // Streams:
  friend inline std::istream& operator>>(std::istream& is, Point2D& point) {
    return is >> point.x >> point.y;
  }

  friend inline std::ostream& operator<<(std::ostream& os, const Point2D& point) {
    os << '(' << point.x << ", " << point.y << ')';
    return os;
  }
};

Point2D() -> Point2D<double>;

template <class T, class U>
Point2D(T, U) -> Point2D<std::common_type_t<T, U>>;

using Point2Df = Point2D<float>;
using Point2Dd = Point2D<double>;
using Point2Di = Point2D<int>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT2D_H

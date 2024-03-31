#ifndef ROBOCIN_GEOMETRY_POINT2D_H
#define ROBOCIN_GEOMETRY_POINT2D_H

#include "robocin/geometry/internal/point2d_internal.h"
#include "robocin/utility/internal/fuzzy_compare_internal.h"

#include <cmath>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace robocin {

template <class T>
struct Point2D {
 public:
  // Member types ----------------------------------------------------------------------------------
  using value_type = T;
  using reference = value_type&;
  using size_type = std::size_t;
  using iterator = point2d_internal::iterator<Point2D>;
  using const_iterator = point2d_internal::iterator<const Point2D>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Friendships -----------------------------------------------------------------------------------
  template <class U>
  friend struct Point2D;

  // Members ---------------------------------------------------------------------------------------
  value_type x, y;

  // Static constructors ---------------------------------------------------------------------------
  static consteval Point2D origin() { return Point2D(); }
  static constexpr Point2D fromPolar(value_type angle)
    requires(std::floating_point<value_type>)
  {
    return Point2D(std::cos(angle), std::sin(angle));
  }
  static constexpr Point2D fromPolar(value_type angle, value_type length)
    requires(std::floating_point<value_type>)
  {
    return Point2D(std::cos(angle) * length, std::sin(angle) * length);
  }

  // Constructors ----------------------------------------------------------------------------------
  constexpr Point2D() : x(0), y(0) {}
  constexpr Point2D(const Point2D& point) = default;
  constexpr Point2D(Point2D&& point) noexcept = default;
  constexpr Point2D(value_type x, value_type y) : x(x), y(y) {} // NOLINT(*swappable*)
  constexpr explicit Point2D(const point2d_internal::StructPoint auto& point) :
      x(static_cast<value_type>(point.x)),
      y(static_cast<value_type>(point.y)) {}
  constexpr explicit Point2D(const point2d_internal::ClassPoint auto& point) :
      x(static_cast<value_type>(point.x())),
      y(static_cast<value_type>(point.y())) {}
  template <class U, class V>
  constexpr explicit Point2D(const std::pair<U, V>& pair) :
      x(static_cast<value_type>(pair.first)),
      y(static_cast<value_type>(pair.second)) {}

  // Destructor ------------------------------------------------------------------------------------
  constexpr ~Point2D() = default;

  // Assignment operators --------------------------------------------------------------------------
  constexpr Point2D& operator=(const Point2D& other) = default;
  constexpr Point2D& operator=(Point2D&& other) noexcept = default;

  // Validators ------------------------------------------------------------------------------------
  [[nodiscard]] constexpr bool isNull() const {
    return fuzzy_compare_internal::fuzzyIsZero(x) and fuzzy_compare_internal::fuzzyIsZero(y);
  }

  // Arithmetic-assignment operators ---------------------------------------------------------------
  inline constexpr Point2D& operator+=(const Point2D& other) {
    return x += other.x, y += other.y, *this;
  }
  inline constexpr Point2D& operator-=(const Point2D& other) {
    return x -= other.x, y -= other.y, *this;
  }
  inline constexpr Point2D& operator*=(value_type factor) {
    return x *= factor, y *= factor, *this;
  }
  inline constexpr Point2D& operator/=(value_type factor) {
    return x /= factor, y /= factor, *this;
  }

  // Arithmetic operators --------------------------------------------------------------------------
  inline constexpr Point2D operator+(const Point2D& other) const { return Point2D(*this) += other; }
  inline constexpr Point2D operator-(const Point2D& other) const { return Point2D(*this) -= other; }
  inline constexpr Point2D operator*(value_type factor) const { return Point2D(*this) *= factor; }
  inline constexpr Point2D operator/(value_type factor) const { return Point2D(*this) /= factor; }

  // Arithmetic friend operators -------------------------------------------------------------------
  friend inline constexpr Point2D operator*(value_type factor, const Point2D& point) {
    return Point2D(factor * point.x, factor * point.y);
  }

  // Sign operators --------------------------------------------------------------------------------
  inline constexpr Point2D operator+() const { return *this; }
  inline constexpr Point2D operator-() const { return Point2D(-x, -y); }

  // Comparison operators --------------------------------------------------------------------------
  inline constexpr bool operator==(const Point2D& other) const {
    return fuzzy_compare_internal::fuzzyCmpEqual(x, other.x)
           and fuzzy_compare_internal::fuzzyCmpEqual(y, other.y);
  }

  inline constexpr auto operator<=>(const Point2D& other) const {
    if (auto x_cmp = fuzzy_compare_internal::fuzzyCmpThreeWay(x, other.x); std::is_neq(x_cmp)) {
      return x_cmp;
    }
    return fuzzy_compare_internal::fuzzyCmpThreeWay(y, other.y);
  }

  // Swap ------------------------------------------------------------------------------------------
  constexpr void swap(Point2D& other) noexcept { std::swap(*this, other); }

  // Geometry --------------------------------------------------------------------------------------
  constexpr value_type dot(const Point2D& other) const { return x * other.x + y * other.y; }
  constexpr value_type cross(const Point2D& other) const { return x * other.y - y * other.x; }

  constexpr value_type manhattanLength() const { return std::abs(x) + std::abs(y); }
  constexpr value_type manhattanDistTo(const Point2D& other) const {
    return std::abs(x - other.x) + std::abs(y - other.y);
  }

  constexpr value_type lengthSquared() const { return x * x + y * y; }
  constexpr std::floating_point auto length() const { return std::sqrt(lengthSquared()); }
  constexpr std::floating_point auto norm() const { return std::sqrt(lengthSquared()); }

  constexpr value_type distSquaredTo(const Point2D& other) const {
    return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
  }
  constexpr std::floating_point auto distTo(const Point2D& other) const {
    return std::sqrt(distSquaredTo(other));
  }

  constexpr std::floating_point auto angle() const { return std::atan2(y, x); }
  constexpr std::floating_point auto angleTo(const Point2D& other) const {
    return std::atan2(cross(other), dot(other));
  }

  // Rotations -------------------------------------------------------------------------------------

  constexpr void rotateCW90() & { std::swap(x, y), y = -y; }
  [[nodiscard]] constexpr Point2D rotatedCW90() && { return rotateCW90(), std::move(*this); }
  [[nodiscard]] constexpr Point2D rotatedCW90() const& { return Point2D(*this).rotatedCW90(); }

  constexpr void rotateCCW90() & { std::swap(x, y), x = -x; }
  [[nodiscard]] constexpr Point2D rotatedCCW90() && { return rotateCCW90(), std::move(*this); }
  [[nodiscard]] constexpr Point2D rotatedCCW90() const& { return Point2D(*this).rotatedCCW90(); }

  constexpr void rotateCW(value_type t) &
    requires(std::floating_point<value_type>)
  {
    value_type cos = std::cos(t);
    value_type sin = std::sin(t);

    value_type rx = x * cos + y * sin;
    value_type ry = -x * sin + y * cos;

    x = rx, y = ry;
  }
  [[nodiscard]] constexpr Point2D rotatedCW(value_type t) &&
    requires(std::floating_point<value_type>)
  {
    return rotateCW(t), std::move(*this);
  }
  [[nodiscard]] constexpr Point2D rotatedCW(value_type t) const&
    requires(std::floating_point<value_type>)
  {
    return Point2D(*this).rotatedCW(t);
  }

  constexpr void rotateCCW(value_type t) &
    requires(std::floating_point<value_type>)
  {
    value_type cos = std::cos(t);
    value_type sin = std::sin(t);

    value_type rx = x * cos - y * sin;
    value_type ry = x * sin + y * cos;

    x = rx, y = ry;
  }
  [[nodiscard]] constexpr Point2D rotatedCCW(value_type t) &&
    requires(std::floating_point<value_type>)
  {
    return rotateCCW(t), std::move(*this);
  }
  [[nodiscard]] constexpr Point2D rotatedCCW(value_type t) const&
    requires(std::floating_point<value_type>)
  {
    return Point2D(*this).rotatedCCW(t);
  }

  // Resizing and normalization --------------------------------------------------------------------

  constexpr void resize(value_type t) &
    requires(std::floating_point<value_type>)
  {
    if (value_type norm = this->norm(); not fuzzy_compare_internal::fuzzyIsZero(norm)) {
      x *= t / norm, y *= t / norm;
    }
  }
  [[nodiscard]] constexpr Point2D resized(value_type t) &&
    requires(std::floating_point<value_type>)
  {
    return resize(t), std::move(*this);
  }
  [[nodiscard]] constexpr Point2D resized(value_type t) const&
    requires(std::floating_point<value_type>)
  {
    return Point2D(*this).resized(t);
  }

  constexpr void normalize() &
    requires(std::floating_point<value_type>)
  {
    if (value_type norm = this->norm(); not fuzzy_compare_internal::fuzzyIsZero(norm)) {
      x /= norm, y /= norm;
    }
  }
  constexpr void normalize() &
    requires(std::integral<value_type>)
  {
    if (value_type gcd = std::gcd(std::abs(x), std::abs(y))) {
      x /= gcd, y /= gcd;
    }
  }
  [[nodiscard]] constexpr Point2D normalized() && { return normalize(), std::move(*this); }
  [[nodiscard]] constexpr Point2D normalized() const& { return Point2D(*this).normalized(); }

  constexpr void axesNormalize() & {
    x = fuzzy_compare_internal::fuzzyIsZero(x) ? 0 : (x > 0 ? 1 : -1),
    y = fuzzy_compare_internal::fuzzyIsZero(y) ? 0 : (y > 0 ? 1 : -1);
  }
  [[nodiscard]] constexpr Point2D axesNormalized() && { return axesNormalize(), std::move(*this); }
  [[nodiscard]] constexpr Point2D axesNormalized() const& {
    return Point2D(*this).axesNormalized();
  }

  constexpr void transpose() & { std::swap(x, y); }
  [[nodiscard]] constexpr Point2D transposed() && { return transpose(), std::move(*this); }
  [[nodiscard]] constexpr Point2D transposed() const& { return Point2D(*this).transposed(); }

  // Array-like ------------------------------------------------------------------------------------
  [[nodiscard]] static consteval size_type size() { return 2; }

  inline constexpr reference operator[](size_type pos) {
    switch (pos) {
      case 0: return x;
      case 1: return y;
      default: throw std::out_of_range("Point2D operator[]: index out of range.");
    }
  }
  inline constexpr value_type operator[](size_type pos) const {
    switch (pos) {
      case 0: return x;
      case 1: return y;
      default: throw std::out_of_range("Point2D operator[]: index out of range.");
    }
  }

  // Iterators -------------------------------------------------------------------------------------
  constexpr iterator begin() noexcept { return iterator{this, /*index=*/0}; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept {
    return const_iterator{this, /*index=*/0};
  }
  constexpr iterator end() noexcept { return iterator{this, size()}; }
  [[nodiscard]] constexpr const_iterator end() const noexcept {
    return const_iterator{this, size()};
  }

  constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator{end()};
  }
  constexpr reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator{begin()};
  }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
    return const_iterator{this, /*index=*/0};
  }
  [[nodiscard]] constexpr const_iterator cend() const noexcept {
    return const_iterator{this, size()};
  }
  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator{end()};
  }
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator{begin()};
  }

  // Input/Output operators ------------------------------------------------------------------------
  friend inline std::istream& operator>>(std::istream& is, Point2D& point) {
    return is >> point.x >> point.y;
  }

  friend inline std::ostream& operator<<(std::ostream& os, const Point2D& point) {
    return os << "(x = " << point.x << ", y = " << point.y << ")";
  }
};

// Deduction guides --------------------------------------------------------------------------------
Point2D() -> Point2D<double>;

template <class T, class U>
Point2D(T, U) -> Point2D<std::common_type_t<T, U>>;

// Aliases -----------------------------------------------------------------------------------------
using Point2Di = Point2D<std::int32_t>;
using Point2Df = Point2D<float>;
using Point2Dd = Point2D<double>;
using Point2Dld = Point2D<long double>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT2D_H

#ifndef ROBOCIN_GEOMETRY_POINT2D_INTERNAL_H
#define ROBOCIN_GEOMETRY_POINT2D_INTERNAL_H

#include <iterator>
#include <type_traits>

namespace robocin::point2d_internal {

template <class PT>
concept StructPoint = requires(PT point) {
  point.x;
  point.y;
};

template <class PT>
concept ClassPoint = requires(PT point) {
  point.x();
  point.y();
};

template <class PT>
class iterator {
  using point_type = PT;
  using point_pointer = point_type*;

  template <class T>
  using dependent_const_t = std::conditional_t<std::is_const_v<point_type>, const T, T>;

 public:
  // NOLINTNEXTLINE(readability-identifier-naming)
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = dependent_const_t<typename point_type::value_type>;
  using pointer = value_type*;
  using reference = value_type&;

  // Constructors ----------------------------------------------------------------------------------
  constexpr iterator() = default;
  constexpr iterator(const iterator&) = default;
  constexpr iterator(iterator&&) noexcept = default;
  constexpr iterator(point_pointer ptr, difference_type index) : ptr_(ptr), index_(index) {}

  // Assignment operators --------------------------------------------------------------------------
  constexpr iterator& operator=(const iterator&) = default;
  constexpr iterator& operator=(iterator&&) noexcept = default;

  // Destructor ------------------------------------------------------------------------------------
  constexpr ~iterator() = default;

  // Dereference operators -------------------------------------------------------------------------
  constexpr reference operator*() const {
    switch (index_) {
      case 0: return ptr_->x;
      case 1: return ptr_->y;
      default: throw std::out_of_range("Point2D::iterator operator*: index out of range.");
    }
  }

  constexpr reference operator[](difference_type n) const {
    switch (index_ + n) {
      case 0: return ptr_->x;
      case 1: return ptr_->y;
      default: throw std::out_of_range("Point2D::iterator operator[]: index out of range.");
    }
  }

  // Arithmetic-assignment operators ---------------------------------------------------------------
  constexpr iterator& operator+=(difference_type n) { return index_ += n, *this; }
  constexpr iterator& operator-=(difference_type n) { return index_ -= n, *this; }

  // Arithmetic operators --------------------------------------------------------------------------
  constexpr iterator operator+(difference_type n) const { return iterator(*this) += n; }
  constexpr iterator operator-(difference_type n) const { return iterator(*this) -= n; }

  constexpr difference_type operator-(const iterator& other) const { return index_ - other.index_; }

  // Arithmetic friend operator --------------------------------------------------------------------
  friend constexpr iterator operator+(difference_type n, const iterator& it) { return it + n; }

  // Increment and decrement operators -------------------------------------------------------------
  constexpr iterator& operator++() { return ++index_, *this; }
  constexpr iterator operator++(int) { // NOLINT(cert-dcl21-cpp)
    iterator result = *this;
    return ++index_, result;
  }

  constexpr iterator& operator--() { return --index_, *this; }
  constexpr iterator operator--(int) { // NOLINT(cert-dcl21-cpp)
    iterator result = *this;
    return --index_, result;
  }

  // Comparison operators --------------------------------------------------------------------------
  inline constexpr bool operator==(const iterator& other) const = default;
  inline constexpr auto operator<=>(const iterator& other) const = default;

 private:
  point_pointer ptr_{nullptr};
  difference_type index_{0};
};

} // namespace robocin::point2d_internal

#endif // ROBOCIN_GEOMETRY_POINT2D_INTERNAL_H

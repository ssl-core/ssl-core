#ifndef ROBOCIN_MEMORY_OBSERVER_PTR_H
#define ROBOCIN_MEMORY_OBSERVER_PTR_H

#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>

namespace robocin {
namespace observer_ptr_internal {

template <class T>
concept smart_pointerish = requires(T ptr) {
  { ptr.get() } -> std::same_as<typename T::pointer>;
  { ptr.operator*() } -> std::same_as<std::add_lvalue_reference_t<typename T::element_type>>;
  { ptr.operator->() } -> std::same_as<typename T::pointer>;
};

} // namespace observer_ptr_internal

template <typename T>
class observer_ptr { // NOLINT(*naming*)
 public:
  using element_type = T;
  using pointer = std::add_pointer_t<element_type>;

  constexpr observer_ptr() noexcept = default;
  constexpr observer_ptr(std::nullptr_t) noexcept {}          // NOLINT(*explicit*)
  constexpr observer_ptr(pointer ptr) noexcept : ptr_{ptr} {} // NOLINT(*explicit*)

  template <std::convertible_to<pointer> U>
  constexpr observer_ptr(U ptr) noexcept : ptr_{ptr} {} // NOLINT(*explicit*)

  template <observer_ptr_internal::smart_pointerish U>
  constexpr observer_ptr(const U& other) noexcept : ptr_(other.get()) {} // NOLINT(*explicit*)

  [[nodiscard]] constexpr pointer get() const noexcept { return ptr_; }

  constexpr std::add_lvalue_reference_t<element_type> operator*() const
      noexcept(noexcept(*std::declval<pointer>())) {
    return *ptr_;
  }

  constexpr pointer operator->() const noexcept { return ptr_; }
  constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }

  constexpr explicit operator pointer() const noexcept { return ptr_; }
  constexpr bool operator!() const noexcept { return ptr_ == nullptr; }

  void reset(pointer ptr = pointer{}) noexcept { ptr_ = ptr; }

  friend constexpr bool operator==(const observer_ptr& lhs, const observer_ptr& rhs) = default;
  friend constexpr auto operator<=>(const observer_ptr& lhs, const observer_ptr& rhs) = default;

 private:
  pointer ptr_{nullptr};
};

template <class T>
observer_ptr(T*) -> observer_ptr<T>;

template <class T>
observer_ptr(std::unique_ptr<T>) -> observer_ptr<typename std::unique_ptr<T>::element_type>;

template <class T>
observer_ptr(std::shared_ptr<T>) -> observer_ptr<typename std::shared_ptr<T>::element_type>;

} // namespace robocin

namespace std {

template <class T>
struct hash<::robocin::observer_ptr<T>> { // NOLINT(*cert*)
  constexpr size_t operator()(const ::robocin::observer_ptr<T>& ptr) const noexcept {
    return hash<typename ::robocin::observer_ptr<T>::pointer>()(ptr.get());
  }
};

} // namespace std

#endif // ROBOCIN_MEMORY_OBSERVER_PTR_H

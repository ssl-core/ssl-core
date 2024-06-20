#ifndef ROBOCIN_MEMORY_OBJECT_PTR_H
#define ROBOCIN_MEMORY_OBJECT_PTR_H

#include "robocin/memory/internal/memory_internal.h"

#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>

namespace robocin {

template <class T>
class object_ptr { // NOLINT(*naming*)
 public:
  using element_type = T;
  using pointer = std::add_pointer_t<element_type>;

  constexpr object_ptr() noexcept = default;
  constexpr object_ptr(std::nullptr_t) noexcept : object_ptr{} {} // NOLINT(*explicit*)
  constexpr object_ptr(pointer ptr) noexcept : ptr_{ptr} {}       // NOLINT(*explicit*)

  template <std::convertible_to<pointer> U>
  constexpr object_ptr(U ptr) noexcept : ptr_{ptr} {} // NOLINT(*explicit*)

  template <memory_internal::smart_pointerish U>
  constexpr object_ptr(const U& other) noexcept : ptr_{other.get()} {} // NOLINT(*explicit*)

  [[nodiscard]] constexpr pointer get() const noexcept { return ptr_; }

  constexpr std::add_lvalue_reference_t<element_type> operator*() const
      noexcept(noexcept(*std::declval<pointer>())) {
    return *ptr_;
  }

  constexpr pointer operator->() const noexcept { return ptr_; }
  constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }

  constexpr explicit operator pointer() const noexcept { return ptr_; }
  constexpr bool operator!() const noexcept { return ptr_ == nullptr; }

  void reset(pointer ptr = nullptr) noexcept { ptr_ = ptr; }

  friend constexpr bool operator==(const object_ptr& lhs, const object_ptr& rhs) = default;
  friend constexpr auto operator<=>(const object_ptr& lhs, const object_ptr& rhs) = default;

 private:
  pointer ptr_{nullptr};
};

template <class T>
object_ptr(T*) -> object_ptr<T>;

template <class T>
object_ptr(std::unique_ptr<T>) -> object_ptr<typename std::unique_ptr<T>::element_type>;

template <class T>
object_ptr(std::shared_ptr<T>) -> object_ptr<typename std::shared_ptr<T>::element_type>;

} // namespace robocin

namespace std {

template <class T>
struct hash<::robocin::object_ptr<T>> { // NOLINT(*cert*)
  constexpr size_t operator()(const ::robocin::object_ptr<T>& ptr) const noexcept {
    return hash<typename ::robocin::object_ptr<T>::pointer>{}(ptr.get());
  }
};

} // namespace std

#endif // ROBOCIN_MEMORY_OBJECT_PTR_H

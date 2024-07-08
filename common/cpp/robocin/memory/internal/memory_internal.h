#ifndef ROBOCIN_MEMORY_INTERNAL_MEMORY_INTERNAL_H
#define ROBOCIN_MEMORY_INTERNAL_MEMORY_INTERNAL_H

#include <concepts>

namespace robocin::memory_internal {

template <class T>
concept smart_pointerish = requires(T ptr) {
  { ptr.get() } -> std::same_as<typename T::pointer>;
  { ptr.operator*() } -> std::same_as<std::add_lvalue_reference_t<typename T::element_type>>;
  { ptr.operator->() } -> std::same_as<typename T::pointer>;
};

} // namespace robocin::memory_internal

#endif // ROBOCIN_MEMORY_INTERNAL_MEMORY_INTERNAL_H

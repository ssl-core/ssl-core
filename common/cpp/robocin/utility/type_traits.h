#ifndef ROBOCIN_UTILITY_TYPE_TRAITS_H
#define ROBOCIN_UTILITY_TYPE_TRAITS_H

#include <type_traits>

namespace robocin {

template <class... Args>
struct common_floating_point_for_comparison;

template <class T>
class common_floating_point_for_comparison<T> {
  using A = std::enable_if_t<std::is_arithmetic_v<T>, T>;

 public:
  using type = std::conditional_t<std::is_floating_point_v<A>, A, double>;
};

template <class T, class U, class... Args>
class common_floating_point_for_comparison<T, U, Args...> {
  using A = std::enable_if_t<std::is_arithmetic_v<T>, T>;
  using B = std::enable_if_t<std::is_arithmetic_v<U>, U>;

  using F = std::conditional_t<std::is_floating_point_v<A>, A, double>;
  using G = std::conditional_t<std::is_floating_point_v<B>, B, double>;

  using H = std::conditional_t<sizeof(F) < sizeof(G), F, G>;

 public:
  using type = typename common_floating_point_for_comparison<H, Args...>::type;
};

template <class... Args>
using common_floating_point_for_comparison_t =
    typename common_floating_point_for_comparison<Args...>::type;

} // namespace robocin

#endif // ROBOCIN_UTILITY_TYPE_TRAITS_H

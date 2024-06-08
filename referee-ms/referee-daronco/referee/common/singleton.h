#ifndef REFEREE_COMMON_SINGLETON_H
#define REFEREE_COMMON_SINGLETON_H

#include <concepts>

namespace gateway {

template <std::default_initializable T>
class Singleton {
  Singleton() = default;

 public:
  using value_type = T;
  using reference = T&;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;

  ~Singleton() = default;

  static reference get() noexcept {
    static value_type value{};
    return value;
  }
};

} // namespace gateway

#endif // REFEREE_COMMON_SINGLETON_H

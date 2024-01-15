#ifndef ROBOCIN_UTILITY_SINGLETON_H
#define ROBOCIN_UTILITY_SINGLETON_H

#include <concepts>

namespace robocin {

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

template <class Key, std::default_initializable Value>
class SingletonMap {
  SingletonMap() = default;

 public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = mapped_type;
  using reference = value_type&;

  SingletonMap(const SingletonMap&) = delete;
  SingletonMap& operator=(const SingletonMap&) = delete;
  SingletonMap(SingletonMap&&) = delete;
  SingletonMap& operator=(SingletonMap&&) = delete;

  ~SingletonMap() = default;

  static reference get() noexcept {
    static value_type value{};
    return value;
  }
};

} // namespace robocin

#endif // ROBOCIN_UTILITY_SINGLETON_H

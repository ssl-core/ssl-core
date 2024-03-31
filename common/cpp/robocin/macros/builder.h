#ifndef ROBOCIN_MACROS_BUILDER_H
#define ROBOCIN_MACROS_BUILDER_H

#include "robocin/macros/internal/builder_internal.h" // IWYU pragma: export

#ifndef ROBOCIN_UNDEF_BUILDER_MACROS

#define BuildableWith(...) ROBOCIN_BUILDER_BUILDABLE_WITH(__VA_ARGS__)
#define BuilderOf(...)     ROBOCIN_BUILDER_BUILDER_OF(__VA_ARGS__)
#define Getter(kind, ...)  __ROBOCIN_BUILDER_GETTER_##kind(__VA_ARGS__)
#define Setter(...)        ROBOCIN_BUILDER_SETTER(__VA_ARGS__)
#define Adder(...)         ROBOCIN_BUILDER_ADDER(__VA_ARGS__)

#define __ROBOCIN_BUILDER_GETTER_Default(...)  ROBOCIN_BUILDER_GETTER_DEFAULT(__VA_ARGS__)
#define __ROBOCIN_BUILDER_GETTER_Optional(...) ROBOCIN_BUILDER_GETTER_OPTIONAL(__VA_ARGS__)
#define __ROBOCIN_BUILDER_GETTER_Repeated(...) ROBOCIN_BUILDER_GETTER_REPEATED(__VA_ARGS__)
// #define ROBOCIN_BUILDER_GETTER_Variant(...)  ROBOCIN_BUILDER_GETTER_VARIANT(__VA_ARGS__)
// #define ROBOCIN_BUILDER_GETTER_Lazy(...)     ROBOCIN_BUILDER_GETTER_LAZY(__VA_ARGS__)

#endif

#define ROBOCIN_BUILDER_BUILDABLE_WITH(clazz) /* NOLINT(*naming*, *macro*) */                      \
 public:                                                                                           \
  class clazz; /* NOLINT(*bugprone*) */                                                            \
  using builder_type = clazz;                                                                      \
                                                                                                   \
  template <std::same_as<builder_type> R = builder_type>                                           \
  auto toBuilder()&& -> R {                                                                        \
    return R::from(std::move(*this));                                                              \
  }                                                                                                \
  template <std::same_as<builder_type> R = builder_type>                                           \
  [[nodiscard]] auto toBuilder() const& -> R {                                                     \
    return R::from(*this);                                                                         \
  }                                                                                                \
                                                                                                   \
  static_assert(true, "expected a semicolon after 'BuildableWith(" #clazz ")'.")

#define ROBOCIN_BUILDER_BUILDER_OF(clazz) /* NOLINT(*naming*, *macro*) */                          \
 public:                                                                                           \
  clazz _M_instance; /* NOLINT(*naming*) */                                                        \
                                                                                                   \
  template <class T>                                                                               \
  static auto from(T&& instance) -> builder_type {                                                 \
    return {._M_instance = std::forward<T>(instance)};                                             \
  }                                                                                                \
                                                                                                   \
  [[nodiscard]] clazz build()&& { return std::move(_M_instance); }                                 \
  [[nodiscard]] clazz build() const& { return _M_instance; }                                       \
                                                                                                   \
  static_assert(std::same_as<builder_type, typename clazz::builder_type>,                          \
                "'" #clazz "' must be buildable and this builder must be its builder: please add " \
                "'BuildableWith(" #clazz ")' to the class definition.")

#define ROBOCIN_BUILDER_SETTER(name) /* NOLINT(*naming*) */                                        \
 public:                                                                                           \
  auto set##name(const decltype(_M_instance._M_##name)& cref_value)&->builder_type& {              \
    _M_instance._M_##name = cref_value;                                                            \
    return *this;                                                                                  \
  }                                                                                                \
  [[nodiscard]] auto set##name(                                                                    \
      const decltype(_M_instance._M_##name)& cref_value)&&->builder_type {                         \
    _M_instance._M_##name = cref_value;                                                            \
    return std::move(*this);                                                                       \
  }                                                                                                \
  auto set##name(decltype(_M_instance._M_##name)&& rvalue)&->builder_type& {                       \
    _M_instance._M_##name = std::move(rvalue);                                                     \
    return *this;                                                                                  \
  }                                                                                                \
  [[nodiscard]] auto set##name(decltype(_M_instance._M_##name)&& rvalue)&&->builder_type {         \
    _M_instance._M_##name = std::move(rvalue);                                                     \
    return std::move(*this);                                                                       \
  }                                                                                                \
  template <class T>                                                                               \
  auto set##name(T&& xvalue)&->builder_type& {                                                     \
    _M_instance._M_##name = std::forward<T>(xvalue); /* NOLINT(*decay*) */                         \
    return *this;                                                                                  \
  }                                                                                                \
  template <class T>                                                                               \
  [[nodiscard]] auto set##name(T&& xvalue)&&->builder_type {                                       \
    _M_instance._M_##name = std::forward<T>(xvalue); /* NOLINT(*decay*) */                         \
    return std::move(*this);                                                                       \
  }                                                                                                \
  static_assert(true, "expected a semicolon after 'Setter(" #name ")'.")

#define ROBOCIN_BUILDER_GETTER_DEFAULT(name, ...) /* NOLINT(*naming*) */                           \
 public:                                                                                           \
  template <std::same_as<__VA_ARGS__> R = __VA_ARGS__>                                             \
  [[nodiscard]] ::robocin::builder_internal::getter_result_t<R> get##name() const {                \
    static_assert(not std::same_as<R, bool>, "use 'is" #name "()' for boolean types.");            \
    return _M_##name;                                                                              \
  }                                                                                                \
  template <std::same_as<__VA_ARGS__> R = __VA_ARGS__>                                             \
  [[nodiscard]] auto is##name() const->::robocin::builder_internal::getter_result_t<R> {           \
    static_assert(std::same_as<R, bool>, "use 'get" #name "()' for non-boolean types.");           \
    return _M_##name;                                                                              \
  }                                                                                                \
  static_assert(not robocin::optionalish<__VA_ARGS__>,                                             \
                "use 'Getter(Optional, " #name ", typename " #__VA_ARGS__                          \
                "::value_type)' for optional types.");                                             \
  ROBOCIN_BUILDER_GETTER_DECLARE(name, __VA_ARGS__)

#define ROBOCIN_BUILDER_GETTER_OPTIONAL(name, ...) /* NOLINT(*naming*) */                          \
 public:                                                                                           \
  template <std::same_as<__VA_ARGS__> R = __VA_ARGS__>                                             \
  [[nodiscard]] ::robocin::builder_internal::getter_result_t<std::optional<R>> get##name() const { \
    static_assert(std::default_initializable<R>,                                                   \
                  "'" #__VA_ARGS__ "' must be default initializable: use 'getOptional" #name       \
                  "()' instead.");                                                                 \
    using ::robocin::builder_internal::get_or_default;                                             \
    return get_or_default(_M_##name);                                                              \
  }                                                                                                \
  [[nodiscard]] ::robocin::builder_internal::getter_result_t<bool> has##name() const {             \
    return _M_##name.has_value();                                                                  \
  }                                                                                                \
  [[nodiscard]] ::robocin::builder_internal::get_optional_result_t<std::optional<__VA_ARGS__>>     \
      getOptional##name() const {                                                                  \
    return _M_##name;                                                                              \
  }                                                                                                \
  static_assert(not robocin::optionalish<__VA_ARGS__>,                                             \
                "when using 'Getter(Optional, ...)', the given type must be non-optional: use "    \
                "'Getter(Optional, " #name ", typename " #__VA_ARGS__                              \
                "::value_type)' for optional types.");                                             \
  ROBOCIN_BUILDER_GETTER_DECLARE(name, std::optional<__VA_ARGS__>)

#define ROBOCIN_BUILDER_GETTER_REPEATED(name, ...) /* NOLINT(*naming*) */                          \
 public:                                                                                           \
  [[nodiscard]] ::robocin::builder_internal::getter_result_t<__VA_ARGS__> get##name() const {      \
    return _M_##name;                                                                              \
  }                                                                                                \
  [[nodiscard]] ::robocin::builder_internal::getter_result_t<typename __VA_ARGS__::value_type>     \
      get##name(size_t pos) const {                                                                \
    return _M_##name[pos];                                                                         \
  }                                                                                                \
  [[nodiscard]] ::robocin::builder_internal::getter_result_t<typename __VA_ARGS__::size_type>      \
      get##name##Size() const {                                                                    \
    return _M_##name.size();                                                                       \
  }                                                                                                \
  static_assert(#name[sizeof(#name) - 2] == 's',                                                   \
                "'" #name "' must ends with 's' for repeated types.");                             \
  ROBOCIN_BUILDER_GETTER_DECLARE(name, __VA_ARGS__)

#define ROBOCIN_BUILDER_GETTER_DECLARE(name, ...)                                                  \
  static_assert('A' <= #name[0] and #name[0] <= 'Z',                                               \
                "'" #name "' must starts with an uppercase letter.");                              \
  __VA_ARGS__ _M_##name {} // NOLINT(*naming*, *redundant*)

#endif // ROBOCIN_MACROS_BUILDER_H

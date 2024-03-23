#ifndef DECISION_MACROS_BUILDER_H
#define DECISION_MACROS_BUILDER_H

#include "decision/macros/internal/builder_internal.h" // IWYU pragma: export

#ifndef DECISION_UNDEF_BUILDER_MACROS

#define BuildableWith(...) DECISION_BUILDER_BUILDABLE_WITH(__VA_ARGS__)
#define BuilderOf(...)     DECISION_BUILDER_BUILDER_OF(__VA_ARGS__)
#define Getter(...)        DECISION_BUILDER_GETTER(__VA_ARGS__)
#define Setter(...)        DECISION_BUILDER_SETTER(__VA_ARGS__)
#define Adder(...)         DECISION_BUILDER_ADDER(__VA_ARGS__)

#endif

#define DECISION_BUILDER_BUILDABLE_WITH(clazz) /* NOLINT(*naming*, *macro*) */                     \
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

#define DECISION_BUILDER_BUILDER_OF(clazz) /* NOLINT(*naming*, *macro*) */                         \
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

#define DECISION_BUILDER_GETTER(name, ...) /* NOLINT(*naming*) */                                  \
 public:                                                                                           \
  template <std::same_as<__VA_ARGS__> R = __VA_ARGS__>                                             \
  [[nodiscard]] auto get##name() const->::decision::builder_internal::getter_result_t<R> {         \
    static_assert(not std::same_as<R, bool>, "use 'is" #name "()' for boolean types.");            \
    if constexpr (::decision::builder_internal::optionalish<R>) {                                  \
      static_assert(std::default_initializable<typename R::value_type>,                            \
                    "'" #__VA_ARGS__                                                               \
                    "::value_type' must be default initializable: use 'getOptional" #name          \
                    "()' instead.");                                                               \
                                                                                                   \
      using ::decision::builder_internal::get_or_default;                                          \
      return get_or_default(_M_##name);                                                            \
    } else {                                                                                       \
      return _M_##name;                                                                            \
    }                                                                                              \
  }                                                                                                \
  template <std::same_as<__VA_ARGS__> R = __VA_ARGS__>                                             \
  [[nodiscard]] auto is##name() const->::decision::builder_internal::getter_result_t<R> {          \
    static_assert(std::same_as<R, bool>, "use 'get" #name "()' for non-boolean types.");           \
    return _M_##name;                                                                              \
  }                                                                                                \
  template <std::same_as<bool> R = bool>                                                           \
  [[nodiscard]] auto has##name() const->::decision::builder_internal::getter_result_t<R> {         \
    static_assert(::decision::builder_internal::optionalish<__VA_ARGS__>,                          \
                  "'has" #name "' can only be used with optional types.");                         \
    using ::decision::builder_internal::has_value;                                                 \
    return has_value(_M_##name);                                                                   \
  }                                                                                                \
  template <std::same_as<__VA_ARGS__> R = __VA_ARGS__>                                             \
  [[nodiscard]] auto getOptional##name()                                                           \
      const->::decision::builder_internal::get_optional_result_t<R> {                              \
    static_assert(::decision::builder_internal::optionalish<R>,                                    \
                  "'getOptional" #name "' can only be used with optional types.");                 \
    return _M_##name;                                                                              \
  }                                                                                                \
  static_assert('A' <= #name[0] and #name[0] <= 'Z',                                               \
                "'" #name "' must starts with an uppercase letter.");                              \
  static_assert(not std::ranges::range<__VA_ARGS__>                                                \
                    or (::decision::builder_internal::stringish<__VA_ARGS__>                       \
                        or #name[sizeof(#name) - 2] == 's'),                                       \
                "'" #name "' must ends with 's' for range types (except strings).");               \
  __VA_ARGS__ _M_##name {} // NOLINT(*naming*, *redundant*)

#define DECISION_BUILDER_SETTER(name) /* NOLINT(*naming*) */                                       \
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

#define DECISION_BUILDER_ADDER(name) /* NOLINT(*naming*) */                                        \
 public:                                                                                           \
  static_assert(::decision::builder_internal::pushable<decltype(_M_instance._M_##name##s)>,        \
                "'add" #name "' can only be used with containers that have 'push_back' or "        \
                "'insert' methods.");                                                              \
  auto add##name(                                                                                  \
      const typename decltype(_M_instance._M_##name##s)::value_type& cref_value)&->builder_type& { \
    using ::decision::builder_internal::push_to;                                                   \
    push_to(_M_instance._M_##name##s, cref_value);                                                 \
    return *this;                                                                                  \
  }                                                                                                \
  [[nodiscard]] auto add##name(                                                                    \
      const typename decltype(_M_instance._M_##name##s)::value_type& cref_value)&&->builder_type { \
    using ::decision::builder_internal::push_to;                                                   \
    push_to(_M_instance._M_##name##s, cref_value);                                                 \
    return std::move(*this);                                                                       \
  }                                                                                                \
  auto add##name(                                                                                  \
      typename decltype(_M_instance._M_##name##s)::value_type&& rvalue)&->builder_type& {          \
    using ::decision::builder_internal::push_to;                                                   \
    push_to(_M_instance._M_##name##s, std::move(rvalue));                                          \
    return *this;                                                                                  \
  }                                                                                                \
  [[nodiscard]] auto add##name(                                                                    \
      typename decltype(_M_instance._M_##name##s)::value_type&& rvalue)&&->builder_type {          \
    using ::decision::builder_internal::push_to;                                                   \
    push_to(_M_instance._M_##name##s, std::move(rvalue));                                          \
    return std::move(*this);                                                                       \
  }                                                                                                \
  static_assert(::decision::builder_internal::emplaceable<decltype(_M_instance._M_##name##s)>,     \
                "'add" #name "' can only be used with containers that have 'emplace_back' or "     \
                "'emplace' methods.");                                                             \
  template <class... Args>                                                                         \
  auto add##name(Args&&... args)&->builder_type& {                                                 \
    using ::decision::builder_internal::emplace_to;                                                \
    emplace_to(_M_instance._M_##name##s, std::forward<Args>(args)...);                             \
    return *this;                                                                                  \
  }                                                                                                \
  template <class... Args>                                                                         \
  [[nodiscard]] auto add##name(Args&&... args)&&->builder_type {                                   \
    using ::decision::builder_internal::emplace_to;                                                \
    emplace_to(_M_instance._M_##name##s, std::forward<Args>(args)...);                             \
    return std::move(*this);                                                                       \
  }                                                                                                \
  auto clear##name##s()&->builder_type& {                                                          \
    _M_instance._M_##name##s.clear();                                                              \
    return *this;                                                                                  \
  }                                                                                                \
  [[nodiscard]] auto clear##name##s()&&->builder_type {                                            \
    _M_instance._M_##name##s.clear();                                                              \
    return std::move(*this);                                                                       \
  }                                                                                                \
  static_assert(true, "expected a semicolon after 'Adder(" #name ")'.")

#endif // DECISION_MACROS_BUILDER_H

#ifndef ROBOCIN_PARAMETERS_PARAMETERS_H
#define ROBOCIN_PARAMETERS_PARAMETERS_H

#include "absl/container/btree_map.h"
#include "robocin/geometry/point2d.h"
#include "robocin/geometry/point3d.h"
#include "robocin/utility/singleton.h"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <source_location>
#include <string>
#include <variant>

namespace robocin {

namespace parameters_internal {

// NOLINTBEGIN(*cert*, *identifier*, *naming*, *explicit*, *init*, *array*)
template <std::size_t N>
struct static_string {
  using value_type = char;

  consteval static_string(const value_type (&str)[N]) noexcept { std::copy_n(str, N, _M_data_); }
  [[nodiscard]] consteval operator std::string_view() const noexcept { return {_M_data_, N}; }

  value_type _M_data_[N];
};
// NOLINTEND(*cert*, *identifier*, *naming*, *explicit*, *init*, *array*)

using value_t = std::variant<std::monostate,
                             bool,
                             int32_t,
                             uint32_t,
                             int64_t,
                             uint64_t,
                             float,
                             double,
                             std::string,
                             Point2Df,
                             Point3Df>;

struct Handler {
  absl::btree_map<std::string_view, std::size_t> parameters;
  std::vector<parameters_internal::value_t> values;
};

} // namespace parameters_internal

template <parameters_internal::static_string Str>
class ParamView {
  struct DataInternal {
    std::vector<std::source_location> locations;
    std::size_t index;

    DataInternal() noexcept : index(Singleton<parameters_internal::Handler>::get().values.size()) {
      Singleton<parameters_internal::Handler>::get().parameters.emplace(std::string_view{Str},
                                                                        index);
      Singleton<parameters_internal::Handler>::get().values.emplace_back();
    }
  };

  inline static auto& Data = Singleton<DataInternal>::get();
  inline static auto& Handler = Singleton<parameters_internal::Handler>::get();

 public:
  ParamView() = delete;

  static std::span<const std::source_location> locations() noexcept { return Data.locations; }
  static constexpr std::string_view name() noexcept { return {Str}; }
  static std::size_t index() noexcept {
    return Data.index; //
  }

  static std::invocable auto asBool(std::source_location loc
                                    = std::source_location::current()) noexcept {
    Data.locations.emplace_back(loc);
    return []() -> bool { return {}; };
  }
};

const auto pIp = ParamView<"ip">::asBool();

} // namespace robocin

const auto pIp2 = robocin::ParamView<"ip">::asBool();

#endif // ROBOCIN_PARAMETERS_PARAMETERS_H

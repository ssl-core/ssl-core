#ifndef ROBOCIN_PARAMETERS_PARAMETERS_H
#define ROBOCIN_PARAMETERS_PARAMETERS_H

#include "robocin/geometry/point2d.h"
#include "robocin/geometry/point3d.h"

#include <cstdint>
#include <memory>
#include <robocin/output/log.h>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace robocin::parameters {

using id_type = size_t;
using parameter_type = std::variant<std::monostate,
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

class Value {
 public:
  Value() = default;

  Value(id_type id, std::string_view name, parameter_type value);

  [[nodiscard]] id_type id() const;
  [[nodiscard]] std::string name() const;
  [[nodiscard]] parameter_type value() const;

 private:
  id_type id_{};
  std::string name_;
  parameter_type value_;
};

class IHandlerEngine {
 public:
  IHandlerEngine() = default;

  IHandlerEngine(const IHandlerEngine&) = delete;
  IHandlerEngine& operator=(const IHandlerEngine&) = delete;
  IHandlerEngine(IHandlerEngine&&) noexcept = default;
  IHandlerEngine& operator=(IHandlerEngine&&) noexcept = default;

  virtual ~IHandlerEngine() = default;

  virtual std::string name_of(id_type id) = 0;     // NOLINT(*naming*)
  virtual parameter_type value_of(id_type id) = 0; // NOLINT(*naming*)
  virtual void update(std::span<const Value> values) = 0;
};

class HandlerEngine : public IHandlerEngine {
 public:
  HandlerEngine() = default;

  std::string name_of(id_type id) override;
  parameter_type value_of(id_type id) override;
  void update(std::span<const Value> values) override;
};

class Handler {
  static parameter_type value_of(id_type id); // NOLINT(*naming*)

 public:
  static void set_engine(std::unique_ptr<IHandlerEngine> engine); // NOLINT(*naming*)

  static std::string name_of(id_type id); // NOLINT(*naming*)

  template <class T>
  static T value_or(id_type id, T default_value) { // NOLINT(*naming*)
    parameter_type value = value_of(id);
    if (auto ptr = std::get_if<T>(&value); ptr != nullptr) {
      return *ptr;
    }

    return default_value;
  }
};

template <id_type Id>
class View {
  template <class T>
  class Proxy {
    friend class View;
    explicit constexpr Proxy(T default_value) : default_value_{std::move(default_value)} {}

   public:
    Proxy(const Proxy&) = delete;
    Proxy& operator=(const Proxy&) = delete;
    Proxy(Proxy&&) noexcept = delete;
    Proxy& operator=(Proxy&&) noexcept = delete;

    virtual ~Proxy() = default;

    [[nodiscard]] auto operator()() const {
      using U = std::conditional_t<std::same_as<T, std::string_view>, std::string, T>;
      return Handler::value_or<U>(Id, static_cast<U>(default_value_));
    }
    [[nodiscard]] std::string name() const { return Handler::name_of(Id); }

   private:
    T default_value_;
  };

 public:
  static constexpr std::invocable auto asBool(bool default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asInt32(int32_t default_value) noexcept {
    return Proxy{default_value};
  };

  static constexpr std::invocable auto asUInt32(uint32_t default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asInt64(int64_t default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asUInt64(uint64_t default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asFloat(float default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asDouble(double default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asString(std::string_view default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asPoint2Df(Point2Df default_value) noexcept {
    return Proxy{default_value};
  }

  static constexpr std::invocable auto asPoint3Df(Point3Df default_value) noexcept {
    return Proxy{default_value};
  }
};

} // namespace robocin::parameters

#endif // ROBOCIN_PARAMETERS_PARAMETERS_H

#include "robocin/parameters/parameters.h"

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include "robocin/output/log.h"

#include <mutex>
#include <robocin/utility/singleton.h>
#include <shared_mutex>
#include <vector>

namespace robocin::parameters {
namespace {

// arbitrary large size, we should never reach this value.
constexpr size_t kParametersMaxSize = 100'000ULL;

class HandlerEngineInternal {
 public:
  HandlerEngineInternal() = default;

  void update(std::span<const Value> values) {
    std::unique_lock locker{mutex_};

    for (const Value& value : values) {
      if (value.id() >= parameters_.size()) {
        parameters_.resize(value.id() << 1ULL);
        if (parameters_.size() > kParametersMaxSize) {
          flog("max parameters size reached.");
        }
      }

      parameters_[value.id()] = value;
    }
  }

  [[nodiscard]] std::string name_of(id_type id) const { // NOLINT(*naming*)
    std::shared_lock locker{mutex_};

    if (id >= parameters_.size()) {
      return std::string{};
    }
    return parameters_[id].name();
  }

  [[nodiscard]] parameter_type value_of(id_type id) const { // NOLINT(*naming*)
    std::shared_lock locker{mutex_};

    if (id >= parameters_.size()) {
      return parameter_type{};
    }
    return parameters_[id].value();
  }

 private:
  mutable std::shared_mutex mutex_;

  std::vector<Value> parameters_;
};

using SHandlerEngineInternal = Singleton<HandlerEngineInternal>;

class HandlerInternal {
 public:
  HandlerInternal() : handler_engine_{std::make_unique<HandlerEngine>()} {}

  void set(std::unique_ptr<IHandlerEngine> handler_engine) {
    std::shared_lock locker{mutex_};
    handler_engine_ = std::move(handler_engine);
  }

  IHandlerEngine& get() const {
    std::shared_lock locker{mutex_};
    return *handler_engine_;
  }

 private:
  mutable std::shared_mutex mutex_;

  std::unique_ptr<IHandlerEngine> handler_engine_;
};

using SHandlerInternal = Singleton<HandlerInternal>;

} // namespace

// value:
Value::Value(id_type id, std::string_view name, parameter_type value) :
    id_{id},
    name_{name},
    value_{std::move(value)} {}

id_type Value::id() const { return id_; }

std::string Value::name() const { return name_; }

parameter_type Value::value() const { return value_; }

// handler engine:
std::string HandlerEngine::name_of(id_type id) { return SHandlerEngineInternal::get().name_of(id); }

parameter_type HandlerEngine::value_of(id_type id) {
  return SHandlerEngineInternal::get().value_of(id);
}

void HandlerEngine::update(std::span<const Value> values) {
  SHandlerEngineInternal::get().update(values);
}

// handler:
void Handler::set_engine(std::unique_ptr<IHandlerEngine> engine) {
  SHandlerInternal::get().set(std::move(engine));
}

std::string Handler::name_of(id_type id) { return SHandlerInternal::get().get().name_of(id); }

parameter_type Handler::value_of(id_type id) { return SHandlerInternal::get().get().value_of(id); }

} // namespace robocin::parameters

#endif

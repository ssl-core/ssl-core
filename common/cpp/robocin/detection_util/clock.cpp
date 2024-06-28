#include "robocin/detection_util/clock.h"

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include "robocin/detection_util/timestamp.h"

#include <mutex>
#include <protocols/perception/detection.pb.h>
#include <robocin/utility/singleton.h>
#include <shared_mutex>

namespace robocin::detection_util {
namespace {

class ClockEngineInternal {
 public:
  ClockEngineInternal() = default;

  void update(uint32_t framerate, Timestamp timestamp) {
    std::unique_lock locker{mutex_};
    timestamp_ = timestamp;
    framerate_ = framerate;
  }

  [[nodiscard]] uint32_t framerate() const {
    std::shared_lock locker{mutex_};
    return framerate_;
  }

  [[nodiscard]] Timestamp timestamp() const {
    std::shared_lock locker{mutex_};
    return timestamp_;
  }

 private:
  mutable std::shared_mutex mutex_;

  uint32_t framerate_{0};
  Timestamp timestamp_{/*serial_id=*/0ULL, /*seconds=*/0LL, /*nanos=*/0};
};

using SClockEngineInternal = Singleton<ClockEngineInternal>;

class ClockInternal {
 public:
  ClockInternal() : handler_engine_{std::make_unique<ClockEngine>()} {}

  void set(std::unique_ptr<IClockEngine> handler_engine) {
    std::unique_lock locker{mutex_};
    handler_engine_ = std::move(handler_engine);
  }

  IClockEngine& get() const {
    std::shared_lock locker{mutex_};
    return *handler_engine_;
  }

 private:
  mutable std::shared_mutex mutex_;

  std::unique_ptr<IClockEngine> handler_engine_;
};

using SClockInternal = Singleton<ClockInternal>;

} // namespace

void Clock::set_clock(std::unique_ptr<IClockEngine> engine) {
  SClockInternal::get().set(std::move(engine));
}

uint32_t Clock::framerate() { return SClockInternal::get().get().framerate(); }

Timestamp Clock::now() { return SClockInternal::get().get().now(); };

uint32_t ClockEngine::framerate() { return SClockEngineInternal::get().framerate(); }

Clock::timestamp_type ClockEngine::now() { return SClockEngineInternal::get().timestamp(); }

void ClockEngine::update(uint32_t framerate, Clock::timestamp_type timestamp) {
  SClockEngineInternal::get().update(framerate, timestamp);
}

} // namespace robocin::detection_util

#endif

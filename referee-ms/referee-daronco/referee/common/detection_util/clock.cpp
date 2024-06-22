#include "referee/common/detection_util/clock.h"

#include "referee/common/detection_util/timestamp.h"

#include <mutex>
#include <protocols/perception/detection.pb.h>
#include <robocin/utility/singleton.h>
#include <shared_mutex>

namespace referee::detection_util {
namespace {

class ClockInternal {
 public:
  ClockInternal() = default;

  void update(Timestamp timestamp, uint32_t framerate) {
    std::unique_lock locker{mutex_};
    framerate_ = framerate;
    timestamp_ = timestamp;
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

using SClockInternal = robocin::Singleton<ClockInternal>;

} // namespace

Timestamp Clock::now() { return SClockInternal::get().timestamp(); };

uint32_t Clock::framerate() { return SClockInternal::get().framerate(); }

void Clock::update(const ::protocols::perception::Detection& detection) {
  SClockInternal::get().update(
      {
          detection.serial_id(),
          detection.created_at().seconds(),
          detection.created_at().nanos(),
      },
      detection.framerate());
}

} // namespace referee::detection_util

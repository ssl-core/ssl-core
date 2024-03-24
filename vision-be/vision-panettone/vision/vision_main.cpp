#include <absl/synchronization/mutex.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <condition_variable>
#include <span>
#include <thread>

constexpr double kNanosPerMilli = 1'000'000.0;

std::mutex mutex;
std::condition_variable cv;
std::vector<std::string> packages;

void subscriberRun() {
  while (true) {
    {
      std::lock_guard lock(mutex);

      // polling.
      //

      // subscriber->receive(PubSubMode::DontWait, current));
    }

    cv.notify_one();
  }
}

void publisherRun() {
  // std::unique_ptr<ITopicPublisher> publisher = std::make_unique<ZmqPublisher>();
  // publisher->bind(std::format("ipc:///tmp/channel{}.ipc", service_id));

  while (true) {
    std::unique_lock lock(mutex);
    cv.wait(lock, [&] { return !packages.empty(); });

    // publisher->send("robocin", PubSubMode::Wait, packages.back());

    packages.clear();
  }
}

int main(int argc, char* argv[]) {
  std::span args{argv + 1, argv + argc - 1};

  std::jthread subscriber_thread(subscriberRun);
  std::jthread publisher_thread(publisherRun);

  return 0;
}

#include <string>

#include <absl/synchronization/mutex.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>

#include "mock/common/publish_subscribe.h"
#include "mock/common/utility.h"
#include "mock/common/zmq_publish_subscribe.h"

using ines::ITopicPublisher;
using ines::ITopicSubscriber;
using ines::PubSubMode;
using ines::ZmqPublisher;
using ines::ZmqSubscriber;

int main(int argc, char *argv[]) {
  int service_id = std::atoi(argv[1]);

  std::unique_ptr<ITopicSubscriber> subscriber =
      std::make_unique<ZmqSubscriber>(std::string(argv[1]));
  subscriber->connect("ipc:///tmp/topic" + std::string(argv[1]) + ".ipc");

  std::unique_ptr<ITopicPublisher> publisher = std::make_unique<ZmqPublisher>();
  publisher->bind("ipc:///tmp/vision.ipc" + std::to_string(service_id + 1) +
                  ".ipc");

  while (true) {
    auto start_time = absl::Now();
    std::cout << service_id << ": " << start_time << '\n';

    std::string pck;
    if (service_id != 0) {
      subscriber->receive(PubSubMode::Wait, pck);
    }

    publisher->send(std::to_string(service_id + 1), PubSubMode::Wait, pck);
    auto end_time = absl::Now();
    std::cout << service_id << ": " << end_time << ", diff:"
              << absl::ToInt64Nanoseconds(end_time - start_time) / 1e6 << '\n';
  }

  return 0;
}

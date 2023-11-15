#ifndef INES_COMMON_PUBLISH_SUBSCRIBE_H
#define INES_COMMON_PUBLISH_SUBSCRIBE_H

#include <optional>
#include <string_view>

#include "mock/common/concepts.h"

namespace ines {

enum class PubSubMode : std::uint8_t {
  Wait = 0,
  DontWait = 1
};

class ITopicPublisher {
 public:
  virtual ~ITopicPublisher() = default;

  virtual void bind(std::string_view address) = 0;
  virtual void send(std::string_view topic, PubSubMode mode, std::string_view message) = 0;

  template <protobuf Pb>
  void send(std::string_view topic, PubSubMode mode, const Pb& proto) {
    std::string serialized_message;
    proto.SerializeToString(&serialized_message);
    send(topic, mode, serialized_message);
  }

  auto callable_send(std::string_view topic, PubSubMode mode) { // NOLINT(readability-*)
    return [this, topic, mode](auto message) {
      send(topic, mode, message);
    };
  }
};

class ITopicSubscriber {
 public:
  explicit ITopicSubscriber(std::string_view topic);

  virtual ~ITopicSubscriber() = default;

  virtual void connect(std::string_view address) = 0;
  virtual bool receive(PubSubMode mode, std::string& result) = 0;

  template <protobuf Pb>
  bool receive(PubSubMode mode, Pb& proto) {
    if (std::string str_result; receive(mode, str_result)) {
      proto.ParseFromString(str_result);
      return true;
    }
    return false;
  }

  auto callable_receive(PubSubMode mode) { // NOLINT(readability-*)
    return [this, mode](auto& result) {
      return receive(mode, result);
    };
  }

  [[nodiscard]] std::string_view topic() const { return topic_; }

 private:
  std::string topic_;
};

} // namespace ines

#endif // INES_COMMON_PUBLISH_SUBSCRIBE_H

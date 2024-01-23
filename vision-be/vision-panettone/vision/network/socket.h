#ifndef VISION_NETWORK_SOCKET_H
#define VISION_NETWORK_SOCKET_H

#include <string_view>

namespace vision {

class ISocketSender {
 public:
  virtual ~ISocketSender() = default;
  virtual void send(std::string_view topic, std::string_view message) = 0;
  virtual void close() = 0;
};

class ISocketReceiver {
 public:
  virtual ~ISocketReceiver() = default;
  virtual std::string receive() = 0;
  virtual void close() = 0;
  [[nodiscard]] virtual int fileDescriptor() const = 0;
};
} // namespace vision

#endif // VISION_NETWORK_SOCKET_H

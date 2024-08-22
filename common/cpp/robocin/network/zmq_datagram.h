#ifndef ROBOCIN_NETWORK_ZMQ_DATAGRAM_H
#define ROBOCIN_NETWORK_ZMQ_DATAGRAM_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_zmq) and __robocin_lib_zmq >= 202405L
#if defined(__robocin_lib_cppzmq) and __robocin_lib_cppzmq >= 202405L

#include <string>
#include <string_view>

namespace robocin {

class ZmqDatagram {
 public:
  ZmqDatagram() = default;
  ZmqDatagram(std::string_view topic, std::string_view message);

  [[nodiscard]] std::string_view topic() const;
  [[nodiscard]] std::string_view message() const;
  [[nodiscard]] bool empty() const;

  friend inline bool operator==(const ZmqDatagram& lhs, const ZmqDatagram& rhs) = default;

 private:
  std::string topic_;
  std::string message_;
};

} // namespace robocin

#endif
#endif

#endif // ROBOCIN_NETWORK_ZMQ_DATAGRAM_H

#ifndef ROBOCIN_NETWORK_UDP_MULTICAST_SOCKET_RECEIVER_H
#define ROBOCIN_NETWORK_UDP_MULTICAST_SOCKET_RECEIVER_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_arpa_inet) and __robocin_lib_arpa_inet >= 202405L

#include <string>

namespace robocin {

class IUdpMulticastSocketReceiver {
 public:
  using receive_type = std::string;

  IUdpMulticastSocketReceiver() = default;

  IUdpMulticastSocketReceiver(const IUdpMulticastSocketReceiver&) = delete;
  IUdpMulticastSocketReceiver& operator=(const IUdpMulticastSocketReceiver&) = delete;
  IUdpMulticastSocketReceiver(IUdpMulticastSocketReceiver&&) = default;
  IUdpMulticastSocketReceiver& operator=(IUdpMulticastSocketReceiver&&) = default;

  virtual ~IUdpMulticastSocketReceiver() = default;

  virtual void connect(std::string_view ip_address, int port) const = 0;
  [[nodiscard]] virtual std::string receive() const = 0;
  virtual void close() const = 0;

  [[nodiscard]] virtual int fd() const = 0;
};

class UdpMulticastSocketReceiver : public IUdpMulticastSocketReceiver {
 public:
  explicit UdpMulticastSocketReceiver(size_t size = 1024);

  void connect(std::string_view ip_address, int port) const override;
  [[nodiscard]] std::string receive() const override;
  void close() const override;

  [[nodiscard]] int fd() const override;

 private:
  int fd_;
  size_t size_;
};

} // namespace robocin

#endif

#endif // ROBOCIN_NETWORK_UDP_MULTICAST_SOCKET_RECEIVER_H

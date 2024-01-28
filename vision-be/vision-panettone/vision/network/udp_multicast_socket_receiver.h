

#ifndef VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H
#define VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H

#include <string>

namespace vision {

class IUdpMulticastSocketReceiver {
 public:
  virtual ~IUdpMulticastSocketReceiver() = default;

  virtual void connect(std::string_view ip_address, std::string_view inet_address, int port) const
      = 0;
  [[nodiscard]] virtual std::string receive() const = 0;
  virtual void close() const = 0;

  [[nodiscard]] virtual int fd() const = 0;
};

class UdpMulticastSocketReceiver : public IUdpMulticastSocketReceiver {
 public:
  using receive_type = std::string;

  UdpMulticastSocketReceiver();

  void connect(std::string_view ip_address, std::string_view inet_address, int port) const override;
  [[nodiscard]] std::string receive() const override;
  void close() const override;

  [[nodiscard]] int fd() const override;

 private:
  int fd_;
};

} // namespace vision

#endif // VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H

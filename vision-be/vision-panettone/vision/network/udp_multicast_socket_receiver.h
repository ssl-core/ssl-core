

#ifndef VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H
#define VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H

#include <string>

namespace vision {

class UdpMulticastSocketReceiver {
 public:
  UdpMulticastSocketReceiver();

  void connect(const std::string& ip_address, const std::string& inet_address, int port) const;

  [[nodiscard]] std::string receive() const;

  void close() const;

  [[nodiscard]] int fileDescriptor() const;

 private:
  int socket_;
};

} // namespace vision

#endif // VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H

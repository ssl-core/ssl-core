#include "vision/network/udp_multicast_socket_receiver.h"

#include <arpa/inet.h>
#include <cassert>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace vision {

UdpMulticastSocketReceiver::UdpMulticastSocketReceiver(const std::string& ip_address,
                                                       const std::string& inet_address,
                                                       int port) :
    socket_(socket(AF_INET, SOCK_DGRAM, 0)) {

  int reuse = 1;
  ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  sockaddr_in udp_addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = {.s_addr = ::inet_addr(ip_address.data())},
  };

  assert(::bind(socket_,
                reinterpret_cast<struct sockaddr*>(&udp_addr), // NOLINT(*reinterpret-cast*)
                sizeof(udp_addr))
         != -1);

  ip_mreqn membership{
      .imr_multiaddr = {.s_addr = ::inet_addr(ip_address.data())},
      .imr_address = {.s_addr = ::inet_addr(inet_address.data())},
  };
  ::setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &membership, sizeof(membership));
}

int UdpMulticastSocketReceiver::fileDescriptor() const { return socket_; }

void UdpMulticastSocketReceiver::close() { ::close(socket_); }

std::string UdpMulticastSocketReceiver::receive() {
  std::string message(2048, '\0');
  ::recvfrom(socket_, message.data(), 2048, 0, nullptr, nullptr);
  return message;
}
} // namespace vision

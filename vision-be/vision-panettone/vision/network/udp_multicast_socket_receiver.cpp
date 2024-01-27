#include "vision/network/udp_multicast_socket_receiver.h"

#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <fcntl.h>
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

UdpMulticastSocketReceiver::UdpMulticastSocketReceiver() :
    socket_(socket(AF_INET, SOCK_DGRAM, 0)) {}

void UdpMulticastSocketReceiver::connect(const std::string& ip_address,
                                         const std::string& inet_address,
                                         int port) const {
  int reuse = 1;
  ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  ip_mreqn membership{
      .imr_multiaddr = {.s_addr = ::inet_addr(ip_address.data())},
      .imr_address = {.s_addr = ::inet_addr(inet_address.data())},
  };
  ::setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &membership, sizeof(membership));

  sockaddr_in udp_addr{
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = {.s_addr = ::inet_addr(ip_address.data())},
  };

  assert(::bind(socket_,
                reinterpret_cast<struct sockaddr*>(&udp_addr), // NOLINT(*reinterpret-cast*)
                sizeof(udp_addr))
         != -1);

  int flags = fcntl(socket_, F_GETFL, 0);
  assert(flags != -1);
  assert(fcntl(socket_, F_SETFL, flags | O_NONBLOCK) != -1);
}

int UdpMulticastSocketReceiver::fileDescriptor() const { return socket_; }

void UdpMulticastSocketReceiver::close() const { ::close(socket_); }

std::string UdpMulticastSocketReceiver::receive() const {
  std::string message(2048, '\0');
  ssize_t recv_message = ::recvfrom(socket_, message.data(), 2048, 0, nullptr, nullptr);
  if (recv_message == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return std::string{};
    }
  }
  return message;
}
} // namespace vision

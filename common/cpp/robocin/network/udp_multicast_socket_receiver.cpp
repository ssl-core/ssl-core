#include "robocin/network/udp_multicast_socket_receiver.h"

#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>

namespace vision {

UdpMulticastSocketReceiver::UdpMulticastSocketReceiver(size_t size) :
    fd_(socket(AF_INET, SOCK_DGRAM, 0)),
    size_(size) {}

void UdpMulticastSocketReceiver::connect(std::string_view ip_address,
                                         std::string_view inet_address,
                                         int port) const {
  if (::setsockopt(fd_,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &std::integral_constant<int, 1>::value,
                   sizeof(int))
      == -1) {
    throw std::runtime_error("failed to set SO_REUSEADDR.");
  }

  if (::ip_mreqn membership{
          .imr_multiaddr = {.s_addr = ::inet_addr(std::string{ip_address}.data())},
          .imr_address = {.s_addr = ::inet_addr(std::string{inet_address}.data())},
      };
      ::setsockopt(fd_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &membership, sizeof(membership)) == -1) {
    throw std::runtime_error("failed to set IP_ADD_MEMBERSHIP.");
  }

  if (::sockaddr_in udp_addr{
          .sin_family = AF_INET,
          .sin_port = htons(port),
          .sin_addr = {.s_addr = ::inet_addr(std::string{ip_address}.data())},
      };
      ::bind(fd_,
             reinterpret_cast<::sockaddr*>(&udp_addr), // NOLINT(*reinterpret-cast*)
             sizeof(udp_addr))
      == -1) {
    throw std::runtime_error("failed to bind.");
  }

  int flags = ::fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    throw std::runtime_error("failed to get socket flags.");
  }

  if (::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) == -1) { // NOLINT(*vararg*, *bitwise*)
    throw std::runtime_error("failed to set O_NONBLOCK.");
  }
}

int UdpMulticastSocketReceiver::fd() const { return fd_; }

void UdpMulticastSocketReceiver::close() const { ::close(fd_); }

std::string UdpMulticastSocketReceiver::receive() const {
  std::string message(size_, '\0');
  ssize_t recv_message = ::recvfrom(fd_, message.data(), size_, 0, nullptr, nullptr);
  if (recv_message == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return std::string{};
    }
  }
  return message;
}

} // namespace vision

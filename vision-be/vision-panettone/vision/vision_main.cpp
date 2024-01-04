#include "vision/network/poller.h"
#include "vision/network/socket.h"
#include "vision/network/udp_multicast_socket_receiver.h"
#include "vision/network/zmq_subscriber_socket.h"

#include <arpa/inet.h>
#include <memory>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <zmq.hpp>

using vision::ISocketReceiver;
using vision::Poller;
using vision::UdpMulticastSocketReceiver;
using vision::ZmqSubscriberSocket;

int main() {
  std::unique_ptr<ISocketReceiver> zmq_socket
      = std::make_unique<ZmqSubscriberSocket>("topic", /*n_threads=*/1, "address");
  std::unique_ptr<ISocketReceiver> udp_socket
      = std::make_unique<UdpMulticastSocketReceiver>("ip", "inet", /*port=*/12345);

  std::unique_ptr<Poller> poller = std::make_unique<Poller>();

  poller->add(zmq_socket);
  poller->add(udp_socket);

  while (true) {
    auto message_zmq = poller->recvFrom(zmq_socket);
    auto message_udp = poller->recvFrom(udp_socket);
  }

  poller->close();
  return 0;
}

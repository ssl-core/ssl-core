#ifndef GATEWAY_CONTROLLERS_ASYNCHRONOUS_SOCKETS_CONTROLLER_H
#define GATEWAY_CONTROLLERS_ASYNCHRONOUS_SOCKETS_CONTROLLER_H

#include "robocin/network/zmq_publisher_socket.h"
#include "robocin/network/zmq_subscriber_socket.h"
#include "gateway/controllers/controller.h"

namespace gateway {

class AsynchronousSocketsController : public IController {
 public:
  explicit AsynchronousSocketsController();
  void run() override;

 private:
  robocin::ZmqPublisherSocket publisher_;
  robocin::ZmqSubscriberSocket subscriber_;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLERS_ASYNCHRONOUS_SOCKETS_CONTROLLER_H

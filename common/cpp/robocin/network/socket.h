#include <atomic>
#include <string>

// class ISocket {
//  public:
//   virtual std::atomic_int connect(const std::string& address) = 0;
//   virtual void send(const std::string& data) = 0;
//   virtual std::string receive(int buffer_size) = 0;
//   virtual void close() = 0;
// };

// udp: no bind, precisa do inet tambem (interface de rede)
// zmq: temos topicos no envio

/*
void ZmqPublisher::send(std::string_view topic, PubSubMode mode,
                        std::string_view message) {
  ZmqSendFlags flags = mode == PubSubMode::DontWait ? ZmqSendFlags::dontwait
                                                    : ZmqSendFlags::none;

  zmq::message_t zmq_topic(topic);
  publisher_.send(zmq_topic, zmq::send_flags::sndmore);

  zmq::message_t zmq_message(message);
  publisher_.send(zmq_message, flags);
}
*/

#include "robocin/network/zmq_datagram.h"

namespace robocin {

ZmqDatagram::ZmqDatagram(std::string_view topic, std::string_view message) : // NOLINT(*swappable*)
    topic_(topic),
    message_(message) {}

std::string_view ZmqDatagram::topic() const { return topic_; }

std::string_view ZmqDatagram::message() const { return message_; }

bool ZmqDatagram::empty() const { return topic_.empty() && message_.empty(); }

} // namespace robocin

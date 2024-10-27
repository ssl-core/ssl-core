#ifndef COMMUNICATION_MESSAGING_RECEIVER_PAYLOAD_H
#define COMMUNICATION_MESSAGING_RECEIVER_PAYLOAD_H

#include <protocols/navigation/navigation.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/network/zmq_datagram.h>
#include <span>
#include <vector>

namespace communication {

class Payload {
 public:
  Payload(std::vector<::protocols::third_party::game_controller::Referee> referee,
          std::vector<::protocols::navigation::Navigation> navigation);

  [[nodiscard]] std::span<const ::protocols::navigation::Navigation> getNavigation() const;
  [[nodiscard]] std::span<const ::protocols::third_party::game_controller::Referee>
  getReferee() const;

  [[nodiscard]] bool empty() const;

 private:
  std::vector<::protocols::navigation::Navigation> navigation_;
  std::vector<::protocols::third_party::game_controller::Referee> referee_;
};

} // namespace communication

#endif // COMMUNICATION_MESSAGING_RECEIVER_PAYLOAD_H

#ifndef REFEREE_MESSAGING_RECEIVER_PAYLOAD_H
#define REFEREE_MESSAGING_RECEIVER_PAYLOAD_H

#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/network/zmq_datagram.h>
#include <span>
#include <vector>

namespace referee {

class Payload {
 public:
  Payload(std::vector<::protocols::perception::Detection> detections,
          std::vector<::protocols::third_party::game_controller::Referee> game_controller_referees);

  [[nodiscard]] std::span<const ::protocols::perception::Detection> getDetections() const;

  [[nodiscard]] std::span<const ::protocols::third_party::game_controller::Referee>
  getGameControllerReferees() const;

  [[nodiscard]] bool empty() const;

 private:
  std::vector<::protocols::perception::Detection> detections_;
  std::vector<::protocols::third_party::game_controller::Referee> game_controller_referees_;
};

} // namespace referee

#endif // REFEREE_MESSAGING_RECEIVER_PAYLOAD_H

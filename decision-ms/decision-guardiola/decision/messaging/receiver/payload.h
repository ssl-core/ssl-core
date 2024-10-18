#ifndef DECISION_MESSAGING_RECEIVER_PAYLOAD_H
#define DECISION_MESSAGING_RECEIVER_PAYLOAD_H

#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <robocin/network/zmq_datagram.h>
#include <span>
#include <vector>

namespace decision {

class Payload {
 public:
  Payload(std::vector<::protocols::perception::Detection> detections,
          std::vector<::protocols::referee::GameStatus> game_status);

  [[nodiscard]] std::span<const ::protocols::perception::Detection> getDetections() const;

  [[nodiscard]] std::span<const ::protocols::referee::GameStatus> getGameStatus() const;

  [[nodiscard]] bool empty() const;

 private:
  std::vector<::protocols::perception::Detection> detections_;
  std::vector<::protocols::referee::GameStatus> game_status_;
};

} // namespace decision

#endif // DECISION_MESSAGING_RECEIVER_PAYLOAD_H

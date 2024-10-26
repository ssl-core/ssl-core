#ifndef NAVIGATION_MESSAGING_RECEIVER_PAYLOAD_H
#define NAVIGATION_MESSAGING_RECEIVER_PAYLOAD_H

#include <protocols/behavior/behavior_unification.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <robocin/network/zmq_datagram.h>
#include <span>
#include <vector>

namespace navigation {

class Payload {
 public:
  Payload(std::vector<::protocols::behavior::unification::Behavior> behaviors,
          std::vector<::protocols::perception::Detection> detections);

  [[nodiscard]] std::span<const ::protocols::behavior::unification::Behavior> getBehaviors() const;
  [[nodiscard]] std::span<const ::protocols::perception::Detection> getDetections() const;
  [[nodiscard]] std::span<const ::protocols::referee::GameStatus> getGameStatuses() const;

  [[nodiscard]] bool empty() const;

 private:
  std::vector<::protocols::behavior::unification::Behavior> behaviors_;
  std::vector<::protocols::perception::Detection> detections_;
  std::vector<::protocols::referee::GameStatus> game_statuses_;
};

} // namespace navigation

#endif // NAVIGATION_MESSAGING_RECEIVER_PAYLOAD_H

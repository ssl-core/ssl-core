#include "referee/common/detection_util/clock.h"
#include "referee/processing/game_command/game_command_mapper.h"
#include "referee/processing/game_events/game_events_mapper.h"
#include "referee/processing/game_stage/game_stage_mapper.h"
#include "referee/processing/game_status/game_status_mapper.h"
#include "referee/processing/team_status/team_status_mapper.h"

#include <expected>
#include <print>
#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/network/zmq_subscriber_socket.h>

using referee::GameCommandMapper;
using referee::GameEventsMapper;
using referee::GameStageMapper;
using referee::GameStatusMapper;
using referee::IGameStatusMapper;
using referee::TeamStatusMapper;
using referee::detection_util::Clock;

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

int main() {
  std::unique_ptr<IGameStatusMapper> game_status_mapper
      = std::make_unique<GameStatusMapper>(std::make_unique<TeamStatusMapper>(),
                                           std::make_unique<GameStageMapper>(),
                                           std::make_unique<GameCommandMapper>(),
                                           std::make_unique<GameEventsMapper>());

  rc::Detection detection;
  tp::Referee referee;

  // once per cycle.
  Clock::update(detection);

  rc::GameStatus game_status = game_status_mapper->fromDetectionAndReferee(detection, referee);
  std::println("{}", game_status.DebugString());

  return 0;
}

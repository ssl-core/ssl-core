#include "referee/common/detection_util/clock.h"
#include "referee/processing/game_command/game_command_mapper.h"
#include "referee/processing/game_events/game_events_mapper.h"
#include "referee/processing/game_stage/game_stage_mapper.h"
#include "referee/processing/game_status/game_status_mapper.h"
#include "referee/processing/team_status/team_status_mapper.h"

#include <expected>
#include <memory>
#include <print>
#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/network/zmq_subscriber_socket.h>

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

int main() {
  referee::TeamStatusMapper team_status_mapper;
  referee::GameStageMapper game_stage_mapper;
  referee::GameCommandMapper game_command_mapper;
  referee::GameEventsMapper game_events_mapper;

  auto game_status_mapper
      = std::make_unique<referee::GameStatusMapper>(std::make_unique<referee::TeamStatusMapper>(),
                                                    std::make_unique<referee::GameStageMapper>(),
                                                    std::make_unique<referee::GameCommandMapper>(),
                                                    std::make_unique<referee::GameEventsMapper>());

  rc::Detection detection;
  tp::Referee referee;

  // once per cycle.
  referee::detection_util::Clock::update(detection);

  rc::GameStatus game_status = game_status_mapper->fromDetectionAndReferee(detection, referee);
  std::println("{}", game_status.DebugString());

  std::expected<int, std::string> expected;
  expected = std::unexpected<std::string>("error");
  // std::unexpected("hello");

  return 0;
}

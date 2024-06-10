#include "game_status_mapper.h"
#include "referee/game_command/game_command_mapper.h"
#include "referee/game_events/game_events_mapper.h"
#include "referee/game_stage/game_stage_mapper.h"
#include "referee/team_status/team_status_mapper.h"

#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>

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

  referee::GameStatusMapper game_status_mapper{
      &team_status_mapper,
      &game_stage_mapper,
      &game_command_mapper,
      &game_events_mapper,
  };

  rc::Detection detection;
  tp::Referee referee;

  rc::GameStatus game_status = game_status_mapper.fromDetectionAndReferee(detection, referee);
  game_status.PrintDebugString();

  return 0;
}

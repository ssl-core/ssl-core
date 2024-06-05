#ifndef REFEREE_GAME_COMMAND_GAME_STAGE_MAPPER_H
#define REFEREE_GAME_COMMAND_GAME_STAGE_MAPPER_H

#include <protocols/common/game_stage.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>

namespace referee {

class GameStageMapper {
 public:
  static ::protocols::common::GameStage
  gameStageFromRefereeStage(::protocols::third_party::game_controller::Referee::Stage stage);
};

} // namespace referee

#endif // REFEREE_GAME_COMMAND_GAME_STAGE_MAPPER_H

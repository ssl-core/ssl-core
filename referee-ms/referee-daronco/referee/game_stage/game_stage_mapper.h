#ifndef REFEREE_GAME_COMMAND_GAME_STAGE_MAPPER_H
#define REFEREE_GAME_COMMAND_GAME_STAGE_MAPPER_H

#include <protocols/common/game_stage.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>

namespace referee {

class IGameStageMapper {
 public:
  IGameStageMapper() = default;

  IGameStageMapper(const IGameStageMapper&) = delete;
  IGameStageMapper& operator=(const IGameStageMapper&) = delete;
  IGameStageMapper(IGameStageMapper&&) = default;
  IGameStageMapper& operator=(IGameStageMapper&&) = default;

  virtual ~IGameStageMapper() = default;

  virtual ::protocols::common::GameStage
  fromRefereeStage(::protocols::third_party::game_controller::Referee::Stage stage)
      = 0;
};

class GameStageMapper : public IGameStageMapper {
 public:
  ::protocols::common::GameStage
  fromRefereeStage(::protocols::third_party::game_controller::Referee::Stage stage) override;
};

} // namespace referee

#endif // REFEREE_GAME_COMMAND_GAME_STAGE_MAPPER_H

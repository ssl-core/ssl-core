#ifndef REFEREE_PROCESSING_GAME_STATUS_GAME_STATUS_MAPPER_H
#define REFEREE_PROCESSING_GAME_STATUS_GAME_STATUS_MAPPER_H

#include "referee/processing/game_command/game_command_mapper.h"
#include "referee/processing/game_events/game_events_mapper.h"
#include "referee/processing/game_stage/game_stage_mapper.h"
#include "referee/processing/team_status/team_status_mapper.h"

#include <protocols/common/team.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>

namespace referee {

class IGameStatusMapper {
 public:
  IGameStatusMapper() = default;

  IGameStatusMapper(const IGameStatusMapper&) = delete;
  IGameStatusMapper& operator=(const IGameStatusMapper&) = delete;
  IGameStatusMapper(IGameStatusMapper&&) = default;
  IGameStatusMapper& operator=(IGameStatusMapper&&) = default;

  virtual ~IGameStatusMapper() = default;

  virtual ::protocols::referee::GameStatus
  fromDetectionAndReferee(const ::protocols::perception::Detection& detection,
                          const ::protocols::third_party::game_controller::Referee& referee)
      = 0;
};

class GameStatusMapper : public IGameStatusMapper {
 public:
  GameStatusMapper(std::unique_ptr<ITeamStatusMapper> team_status_mapper,
                   std::unique_ptr<IGameStageMapper> game_stage_mapper,
                   std::unique_ptr<IGameCommandMapper> game_command_mapper,
                   std::unique_ptr<IGameEventsMapper> game_events_mapper);

  ::protocols::referee::GameStatus fromDetectionAndReferee(
      const ::protocols::perception::Detection& detection,
      const ::protocols::third_party::game_controller::Referee& referee) override;

 private:
  std::unique_ptr<IGameStageMapper> game_stage_mapper_;
  std::unique_ptr<IGameCommandMapper> game_command_mapper_;
  std::unique_ptr<IGameEventsMapper> game_events_mapper_;
  std::unique_ptr<ITeamStatusMapper> team_status_mapper_;
};

} // namespace referee

#endif // REFEREE_PROCESSING_GAME_STATUS_GAME_STATUS_MAPPER_H

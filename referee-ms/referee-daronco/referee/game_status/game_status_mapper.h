#ifndef REFEREE_GAME_STATUS_GAME_STATUS_MAPPER_H
#define REFEREE_GAME_STATUS_GAME_STATUS_MAPPER_H

#include "referee/game_command/game_command_mapper.h"
#include "referee/game_event/game_event_mapper.h"
#include "referee/game_stage/game_stage_mapper.h"
#include "referee/team_status/team_status_mapper.h"

#include <google/protobuf/arena.h>
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
  IGameStatusMapper(IGameStatusMapper&&) = delete;
  IGameStatusMapper& operator=(IGameStatusMapper&&) = delete;

  virtual ~IGameStatusMapper() = default;

  virtual ::robocin::object_ptr<::protocols::referee::GameStatus>
  gameCommandFromDetectionAndReferee(
      const ::protocols::perception::Detection& detection,
      const ::protocols::third_party::game_controller::Referee& referee);
};

class GameStatusMapper : public IGameStatusMapper {
 public:
  explicit GameStatusMapper(::robocin::object_ptr<ITeamStatusMapper> team_status_mapper,
                            ::robocin::object_ptr<IGameStageMapper> game_stage_mapper,
                            ::robocin::object_ptr<IGameCommandMapper> game_command_mapper,
                            ::robocin::object_ptr<IGameEventMapper> game_event_mapper,
                            ::robocin::object_ptr<::google::protobuf::Arena> arena);

  ::robocin::object_ptr<::protocols::referee::GameStatus> gameCommandFromDetectionAndReferee(
      const ::protocols::perception::Detection& detection,
      const ::protocols::third_party::game_controller::Referee& referee) override;

 private:
  ::robocin::object_ptr<IGameStageMapper> game_stage_mapper_;
  ::robocin::object_ptr<IGameCommandMapper> game_command_mapper_;
  ::robocin::object_ptr<IGameEventMapper> game_event_mapper_;
  ::robocin::object_ptr<ITeamStatusMapper> team_status_mapper_;

  ::robocin::object_ptr<::google::protobuf::Arena> arena_;
};

} // namespace referee

#endif // REFEREE_GAME_STATUS_GAME_STATUS_MAPPER_H

#ifndef REFEREE_GAME_COMMAND_GAME_COMMAND_MAPPER_H
#define REFEREE_GAME_COMMAND_GAME_COMMAND_MAPPER_H

#include "referee/detection_util/elapsed_timer.h"

#include <google/protobuf/arena.h>
#include <protocols/common/game_command.pb.h>
#include <protocols/common/game_event.pb.h>
#include <protocols/common/game_stage.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>

namespace referee {

class IGameCommandMapper {
 public:
  IGameCommandMapper() = default;

  IGameCommandMapper(const IGameCommandMapper&) = delete;
  IGameCommandMapper& operator=(const IGameCommandMapper&) = delete;
  IGameCommandMapper(IGameCommandMapper&&) = delete;
  IGameCommandMapper& operator=(IGameCommandMapper&&) = delete;

  virtual ~IGameCommandMapper() = default;

  virtual ::robocin::object_ptr<::protocols::common::GameCommand>
  gameCommandFromDetectionAndReferee(
      ::robocin::object_ptr<const ::protocols::perception::Detection> detection,
      ::robocin::object_ptr<const ::protocols::third_party::game_controller::Referee> referee);
};

class GameCommandMapper : public IGameCommandMapper {
 public:
  explicit GameCommandMapper(::robocin::object_ptr<::google::protobuf::Arena> arena);

  ::robocin::object_ptr<::protocols::common::GameCommand> gameCommandFromDetectionAndReferee(
      ::robocin::object_ptr<const ::protocols::perception::Detection> detection,
      ::robocin::object_ptr<const ::protocols::third_party::game_controller::Referee> referee)
      override;

 private:
  ::robocin::object_ptr<::google::protobuf::Arena> arena_;

  // TODO(joseviccruz): maybe replace by current_action_time_remaining from third party referee.
  detection_util::ElapsedTimer kickoff_elapsed_timer_{/*started=*/false};
  ::protocols::common::Team team_kicking_kickoff_{::protocols::common::Team::TEAM_UNSPECIFIED};

  detection_util::ElapsedTimer direct_free_kick_elapsed_timer_{/*started=*/false};
  ::protocols::common::Team team_kicking_direct_free_kick_{
      ::protocols::common::Team::TEAM_UNSPECIFIED};

  // penalty does not need a timer to make the transition to another command.
  ::protocols::common::Team team_kicking_penalty_{::protocols::common::Team::TEAM_UNSPECIFIED};
};

} // namespace referee

#endif // REFEREE_GAME_COMMAND_GAME_COMMAND_MAPPER_H

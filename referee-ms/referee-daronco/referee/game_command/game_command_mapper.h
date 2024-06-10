#ifndef REFEREE_GAME_COMMAND_GAME_COMMAND_MAPPER_H
#define REFEREE_GAME_COMMAND_GAME_COMMAND_MAPPER_H

#include <protocols/common/game_command.pb.h>
#include <protocols/common/game_event.pb.h>
#include <protocols/common/game_stage.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>

namespace referee {

class IGameCommandMapper {
 public:
  IGameCommandMapper() = default;

  IGameCommandMapper(const IGameCommandMapper&) = delete;
  IGameCommandMapper& operator=(const IGameCommandMapper&) = delete;
  IGameCommandMapper(IGameCommandMapper&&) = default;
  IGameCommandMapper& operator=(IGameCommandMapper&&) = default;

  virtual ~IGameCommandMapper() = default;

  virtual ::protocols::common::GameCommand
  fromDetectionAndReferee(const ::protocols::perception::Detection& detection,
                          const ::protocols::third_party::game_controller::Referee& referee,
                          bool is_next_command)
      = 0;
};

class GameCommandMapper : public IGameCommandMapper {
 public:
  GameCommandMapper() = default;

  ::protocols::common::GameCommand
  fromDetectionAndReferee(const ::protocols::perception::Detection& detection,
                          const ::protocols::third_party::game_controller::Referee& referee,
                          bool is_next_command) override;

 private:
  ::protocols::common::Team team_kicking_kickoff_{::protocols::common::Team::TEAM_UNSPECIFIED};
  ::protocols::common::Team team_kicking_direct_free_kick_{
      ::protocols::common::Team::TEAM_UNSPECIFIED};

  // penalty does not need to check current_action_time_remaining to make the transition to another
  // command.
  ::protocols::common::Team team_kicking_penalty_{::protocols::common::Team::TEAM_UNSPECIFIED};
};

} // namespace referee

#endif // REFEREE_GAME_COMMAND_GAME_COMMAND_MAPPER_H

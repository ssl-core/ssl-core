#ifndef REFEREE_TEAM_STATUS_TEAM_STATUS_H
#define REFEREE_TEAM_STATUS_TEAM_STATUS_H

#include <protocols/common/team.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>

namespace referee {

class ITeamStatusMapper {
 public:
  ITeamStatusMapper() = default;

  ITeamStatusMapper(const ITeamStatusMapper&) = delete;
  ITeamStatusMapper& operator=(const ITeamStatusMapper&) = delete;
  ITeamStatusMapper(ITeamStatusMapper&&) = delete;
  ITeamStatusMapper& operator=(ITeamStatusMapper&&) = delete;

  virtual ~ITeamStatusMapper() = default;

  virtual ::protocols::referee::GameStatus::Team
  fromTeamAndRefereeTeamInfo(::protocols::common::Team team,
                             const ::protocols::third_party::game_controller::Referee& referee)
      = 0;
};

class TeamStatusMapper : public ITeamStatusMapper {
 public:
  TeamStatusMapper() = default;

  ::protocols::referee::GameStatus::Team fromTeamAndRefereeTeamInfo(
      ::protocols::common::Team team,
      const ::protocols::third_party::game_controller::Referee& referee) override;
};

} // namespace referee

#endif // REFEREE_TEAM_STATUS_TEAM_STATUS_H

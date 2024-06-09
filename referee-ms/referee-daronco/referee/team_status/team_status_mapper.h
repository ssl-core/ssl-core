#ifndef REFEREE_TEAM_STATUS_TEAM_STATUS_H
#define REFEREE_TEAM_STATUS_TEAM_STATUS_H

#include <google/protobuf/arena.h>
#include <protocols/common/team.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>

namespace referee {

class ITeamStatusMapper {
 public:
  ITeamStatusMapper() = default;

  ITeamStatusMapper(const ITeamStatusMapper&) = delete;
  ITeamStatusMapper& operator=(const ITeamStatusMapper&) = delete;
  ITeamStatusMapper(ITeamStatusMapper&&) = delete;
  ITeamStatusMapper& operator=(ITeamStatusMapper&&) = delete;

  virtual ~ITeamStatusMapper() = default;

  virtual ::robocin::object_ptr<::protocols::referee::GameStatus::Team>
  fromTeamAndRefereeTeamInfo(::protocols::common::Team team,
                             const ::protocols::third_party::game_controller::Referee& referee);
};

class TeamStatusMapper : public ITeamStatusMapper {
 public:
  explicit TeamStatusMapper(::robocin::object_ptr<::google::protobuf::Arena> arena);

  ::robocin::object_ptr<::protocols::referee::GameStatus::Team> fromTeamAndRefereeTeamInfo(
      ::protocols::common::Team team,
      const ::protocols::third_party::game_controller::Referee& referee) override;

 private:
  ::robocin::object_ptr<::google::protobuf::Arena> arena_;
};

} // namespace referee

#endif // REFEREE_TEAM_STATUS_TEAM_STATUS_H

#include "referee/processing/team_status/team_status_mapper.h"

#include <cstdint>
#include <google/protobuf/repeated_ptr_field.h>
#include <google/protobuf/util/time_util.h>
#include <protocols/common/robot_id.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <utility>

namespace referee {
namespace {

using ::google::protobuf::RepeatedPtrField;
using ::google::protobuf::util::TimeUtil;

namespace rc {

using ETeam = ::protocols::common::Team;

using ::protocols::referee::GameStatus;
using Team = GameStatus::Team;

using RobotId = ::protocols::common::RobotId;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;
using TeamInfo = Referee::TeamInfo;

} // namespace tp

constexpr int kMaxRobots = 11;

const tp::TeamInfo& getTeamInfo(rc::ETeam team, const tp::Referee& referee) {
  bool home_is_blue_team = !referee.blue_team_on_positive_half();

  if (team == rc::ETeam::TEAM_HOME) {
    return home_is_blue_team ? referee.blue() : referee.yellow();
  }
  return home_is_blue_team ? referee.yellow() : referee.blue();
}

rc::RobotId::Color getColor(bool home_is_blue_team, rc::ETeam team) {
  switch (team) {
    case rc::ETeam::TEAM_HOME:
      return home_is_blue_team ? rc::RobotId::COLOR_BLUE : rc::RobotId::COLOR_YELLOW;
    case rc::ETeam::TEAM_AWAY:
      return home_is_blue_team ? rc::RobotId::COLOR_YELLOW : rc::RobotId::COLOR_BLUE;
    default: return rc::RobotId::COLOR_UNSPECIFIED;
  }
  std::unreachable();
}

rc::RobotId makeRobotId(uint32_t number, rc::RobotId::Color color) {
  rc::RobotId result;
  result.set_number(number);
  result.set_color(color);

  return result;
}

RepeatedPtrField<rc::RobotId> getRobotIds(rc::RobotId::Color color) {
  RepeatedPtrField<rc::RobotId> result;

  for (int number = 0; number < kMaxRobots; ++number) {
    *result.Add() = makeRobotId(number, color);
  }

  return result;
}

google::protobuf::Duration durationFromMicros(int64_t microseconds) {
  return TimeUtil::MicrosecondsToDuration(microseconds);
}

} // namespace

rc::Team TeamStatusMapper::fromTeamAndRefereeTeamInfo(rc::ETeam team, const tp::Referee& referee) {
  const tp::TeamInfo& team_info = getTeamInfo(team, referee);

  rc::Team result;

  result.set_name(team_info.name());
  result.set_score(team_info.score());

  /* set robot ids and goalkeeper id */ {
    bool home_is_blue_team = !referee.blue_team_on_positive_half();
    rc::RobotId::Color color = getColor(home_is_blue_team, team);

    *result.mutable_robot_ids() = getRobotIds(color);
    *result.mutable_goalkeeper_id() = makeRobotId(team_info.goalkeeper(), color);
  }

  result.set_yellow_cards(team_info.yellow_cards());
  result.mutable_time_to_expire_active_yellow_cards()->Reserve(team_info.yellow_card_times_size());
  for (uint32_t time_to_expire_yellow_card_in_us : team_info.yellow_card_times()) {
    *result.mutable_time_to_expire_active_yellow_cards()->Add()
        = durationFromMicros(time_to_expire_yellow_card_in_us);
  }

  result.set_red_cards(team_info.red_cards());

  result.set_timeouts_left(team_info.timeouts());
  *result.mutable_total_timeout_time_left() = durationFromMicros(team_info.timeout_time());

  result.set_total_fouls_committed(team_info.foul_counter());

  result.set_consecutive_ball_placement_failures(team_info.ball_placement_failures());
  result.set_is_ball_placement_enabled(team_info.can_place_ball());
  result.set_has_ball_placement_failures_reached_maximum(
      team_info.ball_placement_failures_reached());

  result.set_maximum_allowed_robots(team_info.max_allowed_bots());

  result.set_is_robot_substitution_requested(team_info.bot_substitution_intent());
  result.set_is_robot_substitution_allowed(team_info.bot_substitution_allowed());

  result.set_robot_substitutions_left(team_info.bot_substitutions_left());
  *result.mutable_robot_substitution_time_left()
      = durationFromMicros(team_info.bot_substitution_time_left());

  return result;
}

} // namespace referee

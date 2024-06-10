#include "referee/game_command/game_command_mapper.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <google/protobuf/duration.pb.h>
#include <google/protobuf/stubs/status.h>
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/time_util.h>
#include <protocols/common/game_command.pb.h>
#include <protocols/common/robot_id.pb.h>
#include <protocols/common/team.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>

namespace referee {
namespace {

using ::google::protobuf::util::TimeUtil;
using ::robocin::object_ptr;

// NOLINTBEGIN(*naming*, *magic-numbers*)
constexpr auto pNearTheBallDistance = []() { return 300.0F; };
constexpr auto pSlowerSpeedOfMovingBall = []() { return 300.0F; };
// NOLINTEND(*naming*, *magic-numbers*)

namespace rc {

using Detection = ::protocols::perception::Detection;

using Point2Df = ::protocols::common::Point2Df;
using Point3Df = ::protocols::common::Point3Df;

using GameCommand = ::protocols::common::GameCommand;
using Team = ::protocols::common::Team;
using Robot = ::protocols::perception::Robot;
using RobotId = ::protocols::common::RobotId;

using Interval = GameCommand::Interval;
using Halt = GameCommand::Halt;
using Timeout = GameCommand::Timeout;
using BallPlacement = GameCommand::BallPlacement;
using PrepareKickoff = GameCommand::PrepareKickoff;
using PrepareDirectFreeKick = GameCommand::PrepareDirectFreeKick;
using PreparePenalty = GameCommand::PreparePenalty;
using Kickoff = GameCommand::Kickoff;
using DirectFreeKick = GameCommand::DirectFreeKick;
using Penalty = GameCommand::Penalty;
using InGame = GameCommand::InGame;
using Stop = GameCommand::Stop;

} // namespace rc

namespace tp {

using Point = ::protocols::third_party::game_controller::Referee::Point;

using Referee = ::protocols::third_party::game_controller::Referee;
using RefereeCommand = Referee::Command;
using RefereeStage = Referee::Stage;

using ::protocols::third_party::game_controller::Referee_Command_Name;

} // namespace tp

class RefereeUtil {
 public:
  explicit RefereeUtil(bool is_next_command, const tp::Referee& referee) :
      is_next_command_(is_next_command),
      referee_(&referee) {}

  [[nodiscard]] bool homeIsBlueTeam() const { return !referee_->blue_team_on_positive_half(); }

  [[nodiscard]] rc::Team getTeamFromCommand() const {
    std::string_view referee_command_name = tp::Referee_Command_Name(getCommand());

    if (referee_command_name.contains("BLUE")) {
      return homeIsBlueTeam() ? rc::Team::TEAM_HOME : rc::Team::TEAM_AWAY;
    }
    if (referee_command_name.contains("YELLOW")) {
      return homeIsBlueTeam() ? rc::Team::TEAM_AWAY : rc::Team::TEAM_HOME;
    }

    return rc::Team::TEAM_UNSPECIFIED;
  }

  [[nodiscard]] bool isOutOfGame() const {
    static constexpr std::array kOutOfGameStages = {
        tp::RefereeStage::Referee_Stage_NORMAL_HALF_TIME,
        tp::RefereeStage::Referee_Stage_EXTRA_TIME_BREAK,
        tp::RefereeStage::Referee_Stage_EXTRA_HALF_TIME,
        tp::RefereeStage::Referee_Stage_PENALTY_SHOOTOUT_BREAK,
    };

    return std::ranges::contains(kOutOfGameStages, referee_->stage());
  }

  [[nodiscard]] bool isTimeout() const {
    static constexpr std::array kTimeouts = {
        tp::RefereeCommand::Referee_Command_TIMEOUT_BLUE,
        tp::RefereeCommand::Referee_Command_TIMEOUT_YELLOW,
    };

    return std::ranges::contains(kTimeouts, getCommand());
  }

  [[nodiscard]] bool isPrepareKickoff() const {
    static constexpr std::array kPrepareKickoffs = {
        tp::RefereeCommand::Referee_Command_PREPARE_KICKOFF_BLUE,
        tp::RefereeCommand::Referee_Command_PREPARE_KICKOFF_YELLOW,
    };

    return std::ranges::contains(kPrepareKickoffs, getCommand());
  }

  [[nodiscard]] bool isPreparePenalty() const {
    static constexpr std::array kPreparePenalties = {
        tp::RefereeCommand::Referee_Command_PREPARE_PENALTY_BLUE,
        tp::RefereeCommand::Referee_Command_PREPARE_PENALTY_YELLOW,
    };

    return std::ranges::contains(kPreparePenalties, getCommand());
  }

  [[nodiscard]] bool isBallPlacement() const {
    static constexpr std::array kBallPlacements = {
        tp::RefereeCommand::Referee_Command_BALL_PLACEMENT_BLUE,
        tp::RefereeCommand::Referee_Command_BALL_PLACEMENT_YELLOW,
    };

    return std::ranges::contains(kBallPlacements, getCommand());
  }

  [[nodiscard]] bool isPrepareDirectFreeKick() const {
    static constexpr std::array kPrepareDirectFreeKicks = {
        tp::RefereeCommand::Referee_Command_DIRECT_FREE_BLUE,
        tp::RefereeCommand::Referee_Command_DIRECT_FREE_YELLOW,
    };

    return std::ranges::contains(kPrepareDirectFreeKicks, getCommand());
  }

  [[nodiscard]] bool isHalt() const {
    return getCommand() == tp::RefereeCommand::Referee_Command_HALT;
  }

  [[nodiscard]] bool isStop() const {
    return getCommand() == tp::RefereeCommand::Referee_Command_STOP;
  }

  [[nodiscard]] bool isForceStart() const {
    return getCommand() == tp::RefereeCommand::Referee_Command_FORCE_START;
  }

  [[nodiscard]] bool isNormalStart() const {
    return getCommand() == tp::RefereeCommand::Referee_Command_NORMAL_START;
  }

  [[nodiscard]] bool isCurrentActionTimeUnexpired() const {
    return referee_->current_action_time_remaining() >= 0LL;
  }

  [[nodiscard]] google::protobuf::Duration getCurrentActionTimeRemaining() const {
    return durationFromMicros(referee_->current_action_time_remaining());
  }

  [[nodiscard]] rc::Point2Df getDesignatedPosition() const {
    return point2DfFromPoint(referee_->designated_position());
  }

 private:
  [[nodiscard]] tp::RefereeCommand getCommand() const {
    return is_next_command_ ? referee_->next_command() : referee_->command();
  }

  static google::protobuf::Duration durationFromMicros(int64_t microseconds) {
    return TimeUtil::MicrosecondsToDuration(microseconds);
  }

  static rc::Point2Df point2DfFromPoint(const tp::Point& point) {
    rc::Point2Df result;

    result.set_x(point.x());
    result.set_y(point.y());

    return result;
  }

  bool is_next_command_;
  object_ptr<const tp::Referee> referee_;
};

class FactoryInternal {
  static constexpr std::string_view kTeamUnavailableMessage
      = "team is not home nor away, setting halt.";

 public:
  explicit FactoryInternal(const RefereeUtil referee_util) : referee_util_(&referee_util) {}

  [[nodiscard]] rc::GameCommand makeInterval() const { // NOLINT(*static*)
    rc::GameCommand result;

    *result.mutable_interval() = rc::Interval{};

    return result;
  }

  [[nodiscard]] rc::GameCommand makeHalt() const { // NOLINT(*static*)
    rc::GameCommand result;

    *result.mutable_halt() = rc::Halt{};

    return result;
  }

  rc::GameCommand makeTimeout() {
    rc::GameCommand result;

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_timeout() = rc::Timeout{};
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_timeout() = rc::Timeout{};
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makePrepareKickoff() {
    rc::GameCommand result;

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_prepare_kickoff() = rc::PrepareKickoff{};
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_prepare_kickoff() = rc::PrepareKickoff{};
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makePrepareDirectFreeKick() {
    rc::GameCommand result;

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_prepare_direct_free_kick() = rc::PrepareDirectFreeKick{};
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_prepare_direct_free_kick() = rc::PrepareDirectFreeKick{};
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makePreparePenalty() {
    rc::GameCommand result;

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_prepare_penalty() = rc::PreparePenalty{};
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_prepare_penalty() = rc::PreparePenalty{};
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makeBallPlacement() {
    rc::GameCommand result;

    auto make_ball_placement_fn = [this]() {
      rc::BallPlacement ball_placement;
      *ball_placement.mutable_position() = referee_util_->getDesignatedPosition();
      *ball_placement.mutable_remaining_time() = referee_util_->getCurrentActionTimeRemaining();

      return ball_placement;
    };

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_ball_placement() = make_ball_placement_fn();
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_ball_placement() = make_ball_placement_fn();
        break;
      }
      default: {
        robocin::wlog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makeKickoff(rc::Team team_kicking_kickoff) {
    rc::GameCommand result;

    auto make_kickoff_fn = [this]() {
      rc::Kickoff kickoff;
      *kickoff.mutable_remaining_time() = referee_util_->getCurrentActionTimeRemaining();

      return kickoff;
    };

    switch (team_kicking_kickoff) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_kickoff() = make_kickoff_fn();
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_kickoff() = make_kickoff_fn();
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makeDirectFreeKick(rc::Team team_kicking_free_kick) {
    rc::GameCommand result;

    auto make_direct_free_kick_fn = [this]() {
      rc::DirectFreeKick direct_free_kick;
      *direct_free_kick.mutable_remaining_time() = referee_util_->getCurrentActionTimeRemaining();

      return direct_free_kick;
    };

    switch (team_kicking_free_kick) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_direct_free_kick() = make_direct_free_kick_fn();
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_direct_free_kick() = make_direct_free_kick_fn();
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  rc::GameCommand makePenalty(rc::Team team_kicking_penalty) {
    rc::GameCommand result;

    auto make_penalty_fn = [this]() {
      rc::Penalty penalty;
      *penalty.mutable_remaining_time() = referee_util_->getCurrentActionTimeRemaining();

      return penalty;
    };

    switch (team_kicking_penalty) {
      case rc::Team::TEAM_HOME: {
        *result.mutable_home_penalty() = make_penalty_fn();
        break;
      }
      case rc::Team::TEAM_AWAY: {
        *result.mutable_away_penalty() = make_penalty_fn();
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        *result.mutable_halt() = rc::Halt{};
        break;
      }
    }

    return result;
  }

  [[nodiscard]] rc::GameCommand makeStop() const { // NOLINT(*static*)
    rc::GameCommand result;

    *result.mutable_stop() = rc::Stop{};

    return result;
  }

  [[nodiscard]] rc::GameCommand makeInGame() const { // NOLINT(*static*)
    rc::GameCommand result;

    *result.mutable_in_game() = rc::InGame{};

    return result;
  }

 private:
  object_ptr<const RefereeUtil> referee_util_;
};

class KickingTeamUtil {
 public:
  KickingTeamUtil(const rc::Detection& detection, const RefereeUtil& referee_util) :
      detection_(&detection),
      referee_util_(&referee_util) {}

  void update(rc::Team& team_kicking_kickoff,
              rc::Team& team_kicking_direct_free_kick, // NOLINT(*swappable*)
              rc::Team& team_kicking_penalty) {
    team_kicking_kickoff = getTeamKickingKickoff(team_kicking_kickoff);
    team_kicking_direct_free_kick = getTeamKickingKickoff(team_kicking_direct_free_kick);
    team_kicking_penalty = getTeamKickingPenalty(team_kicking_penalty);
  }

 private:
  [[nodiscard]] rc::Team getTeamKickingKickoff(rc::Team last_team_kicking_kickoff) const {
    if (referee_util_->isPrepareKickoff()) {
      return referee_util_->getTeamFromCommand();
    }
    if (referee_util_->isNormalStart()) {
      if (hasHomeTeamMovedBall() && last_team_kicking_kickoff == rc::Team::TEAM_HOME) {
        return rc::Team::TEAM_UNSPECIFIED;
      }
      if (hasAwayTeamMovedBall() && last_team_kicking_kickoff == rc::Team::TEAM_AWAY) {
        return rc::Team::TEAM_UNSPECIFIED;
      }

      if (referee_util_->isCurrentActionTimeUnexpired()) {
        return last_team_kicking_kickoff;
      }
    }
    return rc::Team::TEAM_UNSPECIFIED;
  }

  [[nodiscard]] rc::Team
  getTeamKickingDirectFreeKick(rc::Team last_team_kicking_direct_free_kick) const {
    if (referee_util_->isPrepareDirectFreeKick()) {
      return referee_util_->getTeamFromCommand();
    }
    if (referee_util_->isNormalStart()) {
      if (hasHomeTeamMovedBall() && last_team_kicking_direct_free_kick == rc::Team::TEAM_HOME) {
        return rc::Team::TEAM_UNSPECIFIED;
      }
      if (hasAwayTeamMovedBall() && last_team_kicking_direct_free_kick == rc::Team::TEAM_AWAY) {
        return rc::Team::TEAM_UNSPECIFIED;
      }

      if (referee_util_->isCurrentActionTimeUnexpired()) {
        return last_team_kicking_direct_free_kick;
      }
    }
    return rc::Team::TEAM_UNSPECIFIED;
  }

  [[nodiscard]] rc::Team getTeamKickingPenalty(rc::Team last_team_kicking_penalty) const {
    if (referee_util_->isPrepareDirectFreeKick()) {
      return referee_util_->getTeamFromCommand();
    }
    if (referee_util_->isNormalStart()) {
      return last_team_kicking_penalty;
    }

    // a spontaneous transition is expected in order to end the penalty.
    return rc::Team::TEAM_UNSPECIFIED;
  }

  [[nodiscard]] bool hasHomeTeamMovedBall() const {
    return teamThatMovedBall() == rc::Team::TEAM_HOME;
  }

  [[nodiscard]] bool hasAwayTeamMovedBall() const {
    return teamThatMovedBall() == rc::Team::TEAM_AWAY;
  }

  [[nodiscard]] rc::Team teamThatMovedBall() const {
    for (const rc::Robot& robot : detection_->robots()) {
      if (isCloseToBall(robot.position()) && isBallMoving()) {
        return getTeamFromRobot(robot);
      }
    }

    return rc::Team::TEAM_UNSPECIFIED;
  }

  [[nodiscard]] bool isCloseToBall(const rc::Point2Df& position) const {
    if (detection_->balls().empty()) {
      return false;
    }

    return distance(position, detection_->balls(0).position()) < pNearTheBallDistance();
  }

  [[nodiscard]] bool isBallMoving() const {
    if (detection_->balls().empty()) {
      return false;
    }

    return norm(detection_->balls(0).velocity()) > pSlowerSpeedOfMovingBall();
  }

  [[nodiscard]] rc::Team getTeamFromRobot(const rc::Robot& robot) const {
    using enum rc::Team;

    if (robot.robot_id().color() == rc::RobotId::COLOR_UNSPECIFIED) {
      return rc::Team::TEAM_UNSPECIFIED;
    }

    if (referee_util_->homeIsBlueTeam()) {
      return robot.robot_id().color() == rc::RobotId::COLOR_BLUE ? TEAM_HOME : TEAM_AWAY;
    }

    return robot.robot_id().color() == rc::RobotId::COLOR_BLUE ? TEAM_AWAY : TEAM_HOME;
  }

  // TODO(josevicruz): replace by common/geometry
  static float norm(const rc::Point3Df& point) {
    return std::sqrt((point.x() * (point.x())) + (point.y() * point.y()) + (point.z() * point.z()));
  }

  // TODO(josevicruz): replace by common/geometry
  static float distance(const rc::Point2Df& lhs, const rc::Point3Df& rhs) {
    return std::sqrt((rhs.x() - lhs.x()) * (rhs.x() - lhs.x())
                     + (rhs.y() - lhs.y()) * (rhs.y() - lhs.y()));
  }

  object_ptr<const rc::Detection> detection_;
  object_ptr<const RefereeUtil> referee_util_;
};

} // namespace

rc::GameCommand GameCommandMapper::fromDetectionAndReferee(const rc::Detection& detection,
                                                           const tp::Referee& referee,
                                                           bool is_next_command) {
  RefereeUtil referee_util{
      is_next_command,
      referee,
  };

  KickingTeamUtil kicking_team_util{
      detection,
      referee_util,
  };

  FactoryInternal factory{
      referee_util,
  };

  kicking_team_util.update(team_kicking_kickoff_,
                           team_kicking_direct_free_kick_,
                           team_kicking_penalty_);

  if (referee_util.isOutOfGame()) {
    return factory.makeInterval();
  }
  if (referee_util.isTimeout()) {
    return factory.makeTimeout();
  }
  if (referee_util.isPrepareKickoff()) {
    return factory.makePrepareKickoff();
  }
  if (referee_util.isPrepareDirectFreeKick()) {
    return factory.makePrepareDirectFreeKick();
  }
  if (referee_util.isPreparePenalty()) {
    return factory.makePreparePenalty();
  }
  if (referee_util.isBallPlacement()) {
    return factory.makeBallPlacement();
  }
  if (referee_util.isStop()) {
    return factory.makeStop();
  }
  if (referee_util.isForceStart()) {
    return factory.makeInGame();
  }
  if (referee_util.isNormalStart()) {
    if (team_kicking_kickoff_ != rc::Team::TEAM_UNSPECIFIED) {
      return factory.makeKickoff(team_kicking_kickoff_);
    }
    if (team_kicking_direct_free_kick_ != rc::Team::TEAM_UNSPECIFIED) {
      return factory.makeDirectFreeKick(team_kicking_direct_free_kick_);
    }
    if (team_kicking_penalty_ != rc::Team::TEAM_UNSPECIFIED) {
      return factory.makePenalty(team_kicking_penalty_);
    }

    return factory.makeInGame();
  }

  if (!referee_util.isHalt()) {
    robocin::elog("the expected command was halt, but '{}' was found.",
                  static_cast<int>(referee.command()));
  }
  return factory.makeHalt();
}

} // namespace referee

#include "referee/game_command/game_command_mapper.h"

#include "referee/detection_util/duration.h"
#include "referee/detection_util/elapsed_timer.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <google/protobuf/arena.h>
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

using ::google::protobuf::Arena;
using ::google::protobuf::util::TimeUtil;
using ::robocin::object_ptr;

using detection_util::Seconds;

// NOLINTBEGIN(*naming*, *magic-numbers*)
constexpr auto pNearTheBallDistance = []() { return 300.0F; };
constexpr auto pSlowerSpeedOfMovingBall = []() { return 300.0F; };
constexpr auto pKickoffTimeout = []() { return 5ULL; };
constexpr auto pDirectFreeKickTimeout = []() { return 5ULL; };
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
  explicit RefereeUtil(const tp::Referee& referee) : referee_(&referee) {}

  [[nodiscard]] bool homeIsBlueTeam() const { return !referee_->blue_team_on_positive_half(); }

  [[nodiscard]] rc::Team getTeamFromCommand() const {
    std::string_view referee_command_name = tp::Referee_Command_Name(referee_->command());

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

    return std::ranges::contains(kTimeouts, referee_->command());
  }

  [[nodiscard]] bool isPrepareKickoff() const {
    static constexpr std::array kPrepareKickoffs = {
        tp::RefereeCommand::Referee_Command_PREPARE_KICKOFF_BLUE,
        tp::RefereeCommand::Referee_Command_PREPARE_KICKOFF_YELLOW,
    };

    return std::ranges::contains(kPrepareKickoffs, referee_->command());
  }

  [[nodiscard]] bool isPreparePenalty() const {
    static constexpr std::array kPreparePenalties = {
        tp::RefereeCommand::Referee_Command_PREPARE_PENALTY_BLUE,
        tp::RefereeCommand::Referee_Command_PREPARE_PENALTY_YELLOW,
    };

    return std::ranges::contains(kPreparePenalties, referee_->command());
  }

  [[nodiscard]] bool isBallPlacement() const {
    static constexpr std::array kBallPlacements = {
        tp::RefereeCommand::Referee_Command_BALL_PLACEMENT_BLUE,
        tp::RefereeCommand::Referee_Command_BALL_PLACEMENT_YELLOW,
    };

    return std::ranges::contains(kBallPlacements, referee_->command());
  }

  [[nodiscard]] bool isPrepareDirectFreeKick() const {
    static constexpr std::array kPrepareDirectFreeKicks = {
        tp::RefereeCommand::Referee_Command_DIRECT_FREE_BLUE,
        tp::RefereeCommand::Referee_Command_DIRECT_FREE_YELLOW,
    };

    return std::ranges::contains(kPrepareDirectFreeKicks, referee_->command());
  }

  [[nodiscard]] bool isHalt() const {
    return referee_->command() == tp::RefereeCommand::Referee_Command_HALT;
  }

  [[nodiscard]] bool isStop() const {
    return referee_->command() == tp::RefereeCommand::Referee_Command_STOP;
  }

  [[nodiscard]] bool isForceStart() const {
    return referee_->command() == tp::RefereeCommand::Referee_Command_FORCE_START;
  }

  [[nodiscard]] bool isNormalStart() const {
    return referee_->command() == tp::RefereeCommand::Referee_Command_NORMAL_START;
  }

  [[nodiscard]] bool isCurrentActionTimeUnexpired() const {
    return referee_->current_action_time_remaining() >= 0LL;
  }

  [[nodiscard]] object_ptr<google::protobuf::Duration>
  getCurrentActionTimeRemaining(object_ptr<Arena> arena) const {
    return durationFromMicros(referee_->current_action_time_remaining(), arena);
  }

  [[nodiscard]] object_ptr<rc::Point2Df> getDesignatedPosition(object_ptr<Arena> arena) const {
    return point2DfFromPoint(referee_->designated_position(), arena);
  }

 private:
  static object_ptr<google::protobuf::Duration> durationFromMicros(int64_t microseconds,
                                                                   object_ptr<Arena> arena) {
    return Arena::Create<google::protobuf::Duration>(
        arena.get(),
        TimeUtil::MicrosecondsToDuration(microseconds));
  }

  static object_ptr<rc::Point2Df> point2DfFromPoint(const tp::Point& point,
                                                    object_ptr<Arena> arena) {
    object_ptr result = Arena::CreateMessage<rc::Point2Df>(arena.get());

    result->set_x(point.x());
    result->set_y(point.y());

    return result;
  }

  object_ptr<const tp::Referee> referee_;
};

class FactoryInternal {
  static constexpr std::string_view kTeamUnavailableMessage
      = "team is not home nor away, setting halt.";

 public:
  FactoryInternal(const RefereeUtil referee_util, object_ptr<Arena> arena) :
      referee_util_(&referee_util),
      arena_(arena) {}

  object_ptr<rc::GameCommand> makeInterval() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    result->unsafe_arena_set_allocated_interval(Arena::CreateMessage<rc::Interval>(arena_.get()));

    return result;
  }

  object_ptr<rc::GameCommand> makeHalt() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));

    return result;
  }

  object_ptr<rc::GameCommand> makeTimeout() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_timeout(
            Arena::CreateMessage<rc::Timeout>(arena_.get()));
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_timeout(
            Arena::CreateMessage<rc::Timeout>(arena_.get()));
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makePrepareKickoff() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_prepare_kickoff(
            Arena::CreateMessage<rc::PrepareKickoff>(arena_.get()));
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_prepare_kickoff(
            Arena::CreateMessage<rc::PrepareKickoff>(arena_.get()));
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makePrepareDirectFreeKick() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_prepare_direct_free_kick(
            Arena::CreateMessage<rc::PrepareDirectFreeKick>(arena_.get()));
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_prepare_direct_free_kick(
            Arena::CreateMessage<rc::PrepareDirectFreeKick>(arena_.get()));
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makePreparePenalty() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_prepare_penalty(
            Arena::CreateMessage<rc::PreparePenalty>(arena_.get()));
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_prepare_penalty(
            Arena::CreateMessage<rc::PreparePenalty>(arena_.get()));
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makeBallPlacement() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    auto make_ball_placement_fn = [this]() {
      object_ptr ball_placement = Arena::CreateMessage<rc::BallPlacement>(arena_.get());
      ball_placement->unsafe_arena_set_allocated_position(
          referee_util_->getDesignatedPosition(arena_).get());
      ball_placement->unsafe_arena_set_allocated_remaining_time(
          referee_util_->getCurrentActionTimeRemaining(arena_).get());

      return ball_placement;
    };

    switch (referee_util_->getTeamFromCommand()) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_ball_placement(make_ball_placement_fn().get());
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_ball_placement(make_ball_placement_fn().get());
        break;
      }
      default: {
        robocin::wlog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makeKickoff(rc::Team team_kicking_kickoff) {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    auto make_kickoff_fn = [this]() {
      object_ptr kickoff = Arena::CreateMessage<rc::Kickoff>(arena_.get());
      kickoff->unsafe_arena_set_allocated_remaining_time(
          referee_util_->getCurrentActionTimeRemaining(arena_).get());

      return kickoff;
    };

    switch (team_kicking_kickoff) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_kickoff(make_kickoff_fn().get());
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_kickoff(make_kickoff_fn().get());
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makeDirectFreeKick(rc::Team team_kicking_free_kick) {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    auto make_direct_free_kick_fn = [this]() {
      object_ptr direct_free_kick = Arena::CreateMessage<rc::DirectFreeKick>(arena_.get());
      direct_free_kick->unsafe_arena_set_allocated_remaining_time(
          referee_util_->getCurrentActionTimeRemaining(arena_).get());

      return direct_free_kick;
    };

    switch (team_kicking_free_kick) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_direct_free_kick(make_direct_free_kick_fn().get());
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_direct_free_kick(make_direct_free_kick_fn().get());
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makePenalty(rc::Team team_kicking_penalty) {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    auto make_penalty_fn = [this]() {
      object_ptr penalty = Arena::CreateMessage<rc::Penalty>(arena_.get());
      penalty->unsafe_arena_set_allocated_remaining_time(
          referee_util_->getCurrentActionTimeRemaining(arena_).get());

      return penalty;
    };

    switch (team_kicking_penalty) {
      case rc::Team::TEAM_HOME: {
        result->unsafe_arena_set_allocated_home_penalty(make_penalty_fn().get());
        break;
      }
      case rc::Team::TEAM_AWAY: {
        result->unsafe_arena_set_allocated_away_penalty(make_penalty_fn().get());
        break;
      }
      default: {
        robocin::elog(kTeamUnavailableMessage);
        result->unsafe_arena_set_allocated_halt(Arena::CreateMessage<rc::Halt>(arena_.get()));
        break;
      }
    }

    return result;
  }

  object_ptr<rc::GameCommand> makeStop() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    result->unsafe_arena_set_allocated_stop(Arena::CreateMessage<rc::Stop>(arena_.get()));

    return result;
  }

  object_ptr<rc::GameCommand> makeInGame() {
    object_ptr result = Arena::CreateMessage<rc::GameCommand>(arena_.get());

    result->unsafe_arena_set_allocated_in_game(Arena::CreateMessage<rc::InGame>(arena_.get()));

    return result;
  }

 private:
  object_ptr<const RefereeUtil> referee_util_;
  object_ptr<Arena> arena_;
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

GameCommandMapper::GameCommandMapper(object_ptr<Arena> arena) : arena_(arena) {}

object_ptr<rc::GameCommand>
GameCommandMapper::gameCommandFromDetectionAndReferee(const rc::Detection& detection,
                                                      const tp::Referee& referee) {
  RefereeUtil referee_util{
      referee,
  };

  FactoryInternal factory{
      referee_util,
      arena_,
  };

  KickingTeamUtil kicking_team_util{
      detection,
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

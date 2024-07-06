#include "referee/processing/game_events/game_events_mapper.h"

#include <google/protobuf/duration.pb.h>
#include <google/protobuf/repeated_ptr_field.h>
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/time_util.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/detection_util/clock.h>
#include <robocin/detection_util/timestamp.h>
#include <robocin/memory/object_ptr.h>

namespace referee {
namespace {

namespace detection_util = ::robocin::detection_util;

using ::google::protobuf::RepeatedPtrField;
using ::google::protobuf::util::TimeUtil;
using ::robocin::object_ptr;

constexpr float kFMillimetersPerMeters = 1e3F;

namespace rc {

using GameEvent = ::protocols::common::GameEvent;
using GameEventsProposal = ::protocols::referee::GameStatus::GameEventsProposal;

using Team = ::protocols::common::Team;
using RobotId = ::protocols::common::RobotId;
using Point2Df = ::protocols::common::Point2Df;

using BallLeftField = GameEvent::BallLeftField;
using BallLeftFieldBoundary = GameEvent::BallLeftFieldBoundary;
using AimlessKick = GameEvent::AimlessKick;
using GoalkeeperHeldBall = GameEvent::GoalkeeperHeldBall;
using RobotTooCloseToDefenseArea = GameEvent::RobotTooCloseToDefenseArea;
using RobotInDefenseArea = GameEvent::RobotInDefenseArea;
using RobotPushedRobot = GameEvent::RobotPushedRobot;
using RobotHeldBallDeliberately = GameEvent::RobotHeldBallDeliberately;
using RobotDribbledBallTooFar = GameEvent::RobotDribbledBallTooFar;
using RobotTippedOver = GameEvent::RobotTippedOver;
using RobotTouchedBallInDefenseArea = GameEvent::RobotTouchedBallInDefenseArea;
using RobotKickedBallTooFast = GameEvent::RobotKickedBallTooFast;
using RobotCrashUnique = GameEvent::RobotCrashUnique;
using RobotCrashDrawn = GameEvent::RobotCrashDrawn;
using RobotTooFastInStop = GameEvent::RobotTooFastInStop;
using RobotTooCloseToKickPoint = GameEvent::RobotTooCloseToKickPoint;
using RobotInterferedBallPlacement = GameEvent::RobotInterferedBallPlacement;
using RobotDoubleTouchedBall = GameEvent::RobotDoubleTouchedBall;
using NoProgressInGame = GameEvent::NoProgressInGame;
using MultipleCards = GameEvent::MultipleCards;
using MultipleFouls = GameEvent::MultipleFouls;
using TooManyRobots = GameEvent::TooManyRobots;
using BallPlacementSucceeded = GameEvent::BallPlacementSucceeded;
using BallPlacementFailed = GameEvent::BallPlacementFailed;
using PenaltyKickFailed = GameEvent::PenaltyKickFailed;
using Goal = GameEvent::Goal;
using RobotSubstitution = GameEvent::RobotSubstitution;
using ChallengeFlag = GameEvent::ChallengeFlag;
using EmergencyStop = GameEvent::EmergencyStop;
using UnsportingBehaviorMinor = GameEvent::UnsportingBehaviorMinor;
using UnsportingBehaviorMajor = GameEvent::UnsportingBehaviorMajor;

} // namespace rc

namespace tp {

using Referee = ::protocols::third_party::game_controller::Referee;
using GameEvent = ::protocols::third_party::game_controller::GameEvent;
using GameEventProposalGroup = ::protocols::third_party::game_controller::GameEventProposalGroup;

using Team = ::protocols::third_party::game_controller::Team;
using Vector2 = ::protocols::third_party::game_controller::Vector2;

using BallLeftField = GameEvent::BallLeftField;
using BoundaryCrossing = GameEvent::BoundaryCrossing;
using AimlessKick = GameEvent::AimlessKick;
using KeeperHeldBall = GameEvent::KeeperHeldBall;
using AttackerTooCloseToDefenseArea = GameEvent::AttackerTooCloseToDefenseArea;
using DefenderInDefenseArea = GameEvent::DefenderInDefenseArea;
using BotPushedBot = GameEvent::BotPushedBot;
using BotHeldBallDeliberately = GameEvent::BotHeldBallDeliberately;
using BotDribbledBallTooFar = GameEvent::BotDribbledBallTooFar;
using BotTippedOver = GameEvent::BotTippedOver;
using AttackerTouchedBallInDefenseArea = GameEvent::AttackerTouchedBallInDefenseArea;
using BotKickedBallTooFast = GameEvent::BotKickedBallTooFast;
using BotCrashUnique = GameEvent::BotCrashUnique;
using BotCrashDrawn = GameEvent::BotCrashDrawn;
using BotTooFastInStop = GameEvent::BotTooFastInStop;
using DefenderTooCloseToKickPoint = GameEvent::DefenderTooCloseToKickPoint;
using BotInterferedPlacement = GameEvent::BotInterferedPlacement;
using AttackerDoubleTouchedBall = GameEvent::AttackerDoubleTouchedBall;
using NoProgressInGame = GameEvent::NoProgressInGame;
using MultipleCards = GameEvent::MultipleCards;
using MultipleFouls = GameEvent::MultipleFouls;
using TooManyRobots = GameEvent::TooManyRobots;
using PlacementSucceeded = GameEvent::PlacementSucceeded;
using PlacementFailed = GameEvent::PlacementFailed;
using PenaltyKickFailed = GameEvent::PenaltyKickFailed;
using Goal = GameEvent::Goal;
using BotSubstitution = GameEvent::BotSubstitution;
using ChallengeFlag = GameEvent::ChallengeFlag;
using EmergencyStop = GameEvent::EmergencyStop;
using UnsportingBehaviorMinor = GameEvent::UnsportingBehaviorMinor;
using UnsportingBehaviorMajor = GameEvent::UnsportingBehaviorMajor;

Team otherTeam(const Team team) {
  switch (team) {
    case Team::BLUE: return Team::YELLOW;
    case Team::YELLOW: return Team::BLUE;
    default: return Team::UNKNOWN;
  }
  std::unreachable();
}

} // namespace tp

class MapperInternal {
 public:
  explicit MapperInternal(bool home_is_blue_team) : home_is_blue_team_(home_is_blue_team) {}

  [[nodiscard]] rc::BallLeftField
  ballLeftFieldFromBallLeftField(const tp::BallLeftField& ball_left_field) const {
    rc::BallLeftField result;

    result.set_by_team(teamFromTeam(ball_left_field.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(ball_left_field.by_team(), ball_left_field.by_bot());
    *result.mutable_left_field_position() = point2DfFromVector2(ball_left_field.location());

    return result;
  }

  [[nodiscard]] rc::BallLeftFieldBoundary
  ballLeftFieldBoundaryFromBoundaryCrossing(const tp::BoundaryCrossing& boundary_crossing) const {
    rc::BallLeftFieldBoundary result;

    result.set_by_team(teamFromTeam(boundary_crossing.by_team()));
    *result.mutable_left_field_boundary_position()
        = point2DfFromVector2(boundary_crossing.location());

    return result;
  }

  [[nodiscard]] rc::AimlessKick
  aimlessKickFromAimlessKick(const tp::AimlessKick& aimless_kick) const {
    rc::AimlessKick result;

    result.set_by_team(teamFromTeam(aimless_kick.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(aimless_kick.by_team(), aimless_kick.by_bot());
    *result.mutable_left_field_position() = point2DfFromVector2(aimless_kick.location());
    *result.mutable_kick_position() = point2DfFromVector2(aimless_kick.kick_location());

    return result;
  }

  [[nodiscard]] rc::GoalkeeperHeldBall
  goalkeeperHeldBallFromKeeperHeldBall(const tp::KeeperHeldBall& keeper_held_ball) const {
    rc::GoalkeeperHeldBall result;

    result.set_by_team(teamFromTeam(keeper_held_ball.by_team()));
    *result.mutable_ball_position() = point2DfFromVector2(keeper_held_ball.location());
    *result.mutable_duration() = durationFromSeconds(keeper_held_ball.duration());

    return result;
  }

  [[nodiscard]] rc::RobotTooCloseToDefenseArea
  robotTooCloseToDefenseAreaFromAttackerTooCloseToDefenseArea(
      const tp::AttackerTooCloseToDefenseArea& attacker_too_close_to_defense_area) const {
    rc::RobotTooCloseToDefenseArea result;

    result.set_by_team(teamFromTeam(attacker_too_close_to_defense_area.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(attacker_too_close_to_defense_area.by_team(),
                                   attacker_too_close_to_defense_area.by_bot());
    *result.mutable_robot_position()
        = point2DfFromVector2(attacker_too_close_to_defense_area.location());
    result.set_distance_to_defense_area(attacker_too_close_to_defense_area.distance()
                                        * kFMillimetersPerMeters);
    *result.mutable_ball_position()
        = point2DfFromVector2(attacker_too_close_to_defense_area.ball_location());

    return result;
  }

  [[nodiscard]] rc::RobotInDefenseArea robotInDefenseAreaFromDefenderInDefenseArea(
      const tp::DefenderInDefenseArea& defender_in_defense_area) const {
    rc::RobotInDefenseArea result;

    result.set_by_team(teamFromTeam(defender_in_defense_area.by_team()));
    *result.mutable_by_robot() = robotIdFromTeamAndNumber(defender_in_defense_area.by_team(),
                                                          defender_in_defense_area.by_bot());
    *result.mutable_robot_position() = point2DfFromVector2(defender_in_defense_area.location());
    result.set_distance_to_nearest_point_outside_area(defender_in_defense_area.distance()
                                                      * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::RobotPushedRobot
  robotPushedRobotFromBotPushedBot(const tp::BotPushedBot& bot_pushed_bot) const {
    rc::RobotPushedRobot result;

    result.set_by_team(teamFromTeam(bot_pushed_bot.by_team()));
    *result.mutable_violator_robot()
        = robotIdFromTeamAndNumber(bot_pushed_bot.by_team(), bot_pushed_bot.violator());
    *result.mutable_victim_robot()
        = robotIdFromTeamAndNumber(tp::otherTeam(bot_pushed_bot.by_team()),
                                   bot_pushed_bot.victim());
    *result.mutable_position() = point2DfFromVector2(bot_pushed_bot.location());
    result.set_pushed_distance(bot_pushed_bot.pushed_distance() * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::RobotHeldBallDeliberately robotHeldBallDeliberatelyFromBotHeldBallDeliberately(
      const tp::BotHeldBallDeliberately& bot_held_ball_deliberately) const {
    rc::RobotHeldBallDeliberately result;

    result.set_by_team(teamFromTeam(bot_held_ball_deliberately.by_team()));
    *result.mutable_by_robot() = robotIdFromTeamAndNumber(bot_held_ball_deliberately.by_team(),
                                                          bot_held_ball_deliberately.by_bot());
    *result.mutable_ball_position() = point2DfFromVector2(bot_held_ball_deliberately.location());
    *result.mutable_duration() = durationFromSeconds(bot_held_ball_deliberately.duration());

    return result;
  }

  [[nodiscard]] rc::RobotDribbledBallTooFar robotDribbledBallTooFarFromBotDribbledBallTooFar(
      const tp::BotDribbledBallTooFar& bot_dribbled_ball_too_far) const {
    rc::RobotDribbledBallTooFar result;

    result.set_by_team(teamFromTeam(bot_dribbled_ball_too_far.by_team()));
    *result.mutable_by_robot() = robotIdFromTeamAndNumber(bot_dribbled_ball_too_far.by_team(),
                                                          bot_dribbled_ball_too_far.by_bot());
    *result.mutable_start_position() = point2DfFromVector2(bot_dribbled_ball_too_far.start());
    *result.mutable_end_position() = point2DfFromVector2(bot_dribbled_ball_too_far.end());

    return result;
  }

  [[nodiscard]] rc::RobotTippedOver
  robotTippedOverFromBotTippedOver(const tp::BotTippedOver& bot_tipped_over) const {
    rc::RobotTippedOver result;

    result.set_by_team(teamFromTeam(bot_tipped_over.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(bot_tipped_over.by_team(), bot_tipped_over.by_bot());
    *result.mutable_robot_position() = point2DfFromVector2(bot_tipped_over.location());
    *result.mutable_ball_position() = point2DfFromVector2(bot_tipped_over.ball_location());

    return result;
  }

  [[nodiscard]] rc::RobotTouchedBallInDefenseArea
  robotTouchedBallInDefenseAreaFromAttackerTouchedBallInDefenseArea(
      const tp::AttackerTouchedBallInDefenseArea& attacker_touched_ball_in_defense_area) const {
    rc::RobotTouchedBallInDefenseArea result;

    result.set_by_team(teamFromTeam(attacker_touched_ball_in_defense_area.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(attacker_touched_ball_in_defense_area.by_team(),
                                   attacker_touched_ball_in_defense_area.by_bot());
    *result.mutable_robot_position()
        = point2DfFromVector2(attacker_touched_ball_in_defense_area.location());
    result.set_distance_to_nearest_point_outside_area(
        attacker_touched_ball_in_defense_area.distance() * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::RobotKickedBallTooFast robotKickedBallTooFastFromBotKickedBallTooFast(
      const tp::BotKickedBallTooFast& bot_kicked_ball_too_fast) const {
    rc::RobotKickedBallTooFast result;

    result.set_by_team(teamFromTeam(bot_kicked_ball_too_fast.by_team()));
    *result.mutable_by_robot() = robotIdFromTeamAndNumber(bot_kicked_ball_too_fast.by_team(),
                                                          bot_kicked_ball_too_fast.by_bot());
    *result.mutable_ball_at_highest_speed_position()
        = point2DfFromVector2(bot_kicked_ball_too_fast.location());
    result.set_initial_ball_speed(bot_kicked_ball_too_fast.initial_ball_speed()
                                  * kFMillimetersPerMeters);
    result.set_was_chipped_kick(bot_kicked_ball_too_fast.chipped());

    return result;
  }

  [[nodiscard]] rc::RobotCrashUnique
  robotCrashUniqueFromBotCrashUnique(const tp::BotCrashUnique& bot_crash_unique) const {
    rc::RobotCrashUnique result;

    result.set_by_team(teamFromTeam(bot_crash_unique.by_team()));
    *result.mutable_violator_robot()
        = robotIdFromTeamAndNumber(bot_crash_unique.by_team(), bot_crash_unique.violator());
    *result.mutable_victim_robot()
        = robotIdFromTeamAndNumber(tp::otherTeam(bot_crash_unique.by_team()),
                                   bot_crash_unique.victim());
    *result.mutable_crash_position() = point2DfFromVector2(bot_crash_unique.location());
    result.set_crash_speed(bot_crash_unique.crash_speed() * kFMillimetersPerMeters);
    result.set_speed_difference(bot_crash_unique.speed_diff() * kFMillimetersPerMeters);
    result.set_crash_angle(bot_crash_unique.crash_angle());

    return result;
  }

  [[nodiscard]] rc::RobotCrashDrawn
  robotCrashDrawnFromBotCrashDrawn(const tp::BotCrashDrawn& bot_crash_drawn) const {
    rc::RobotCrashDrawn result;

    if (home_is_blue_team_) {
      *result.mutable_home_robot_id()
          = robotIdFromTeamAndNumber(tp::Team::BLUE, bot_crash_drawn.bot_blue());
      *result.mutable_away_robot_id()
          = robotIdFromTeamAndNumber(tp::Team::YELLOW, bot_crash_drawn.bot_yellow());
    } else {
      *result.mutable_home_robot_id()
          = robotIdFromTeamAndNumber(tp::Team::YELLOW, bot_crash_drawn.bot_yellow());
      *result.mutable_away_robot_id()
          = robotIdFromTeamAndNumber(tp::Team::BLUE, bot_crash_drawn.bot_blue());
    }

    *result.mutable_crash_position() = point2DfFromVector2(bot_crash_drawn.location());
    result.set_crash_speed(bot_crash_drawn.crash_speed() * kFMillimetersPerMeters);
    result.set_speed_difference(bot_crash_drawn.speed_diff() * kFMillimetersPerMeters);
    result.set_crash_angle(bot_crash_drawn.crash_angle());

    return result;
  }

  [[nodiscard]] rc::RobotTooFastInStop
  robotTooFastInStopFromBotTooFastInStop(const tp::BotTooFastInStop& bot_too_fast_in_stop) const {
    rc::RobotTooFastInStop result;

    result.set_by_team(teamFromTeam(bot_too_fast_in_stop.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(bot_too_fast_in_stop.by_team(), bot_too_fast_in_stop.by_bot());
    *result.mutable_robot_position() = point2DfFromVector2(bot_too_fast_in_stop.location());
    result.set_robot_speed(bot_too_fast_in_stop.speed() * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::RobotTooCloseToKickPoint
  robotTooCloseToKickPointFromDefenderTooCloseToKickPoint(
      const tp::DefenderTooCloseToKickPoint& defender_too_close_to_kick_point) const {
    rc::RobotTooCloseToKickPoint result;

    result.set_by_team(teamFromTeam(defender_too_close_to_kick_point.by_team()));
    *result.mutable_by_robot()
        = robotIdFromTeamAndNumber(defender_too_close_to_kick_point.by_team(),
                                   defender_too_close_to_kick_point.by_bot());
    *result.mutable_robot_position()
        = point2DfFromVector2(defender_too_close_to_kick_point.location());
    result.set_robot_distance(defender_too_close_to_kick_point.distance() * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::RobotInterferedBallPlacement
  robotInterferedBallPlacementFromBotInterferedPlacement(
      const tp::BotInterferedPlacement& bot_interfered_placement) const {
    rc::RobotInterferedBallPlacement result;

    result.set_by_team(teamFromTeam(bot_interfered_placement.by_team()));
    *result.mutable_by_robot() = robotIdFromTeamAndNumber(bot_interfered_placement.by_team(),
                                                          bot_interfered_placement.by_bot());
    *result.mutable_robot_position() = point2DfFromVector2(bot_interfered_placement.location());

    return result;
  }

  [[nodiscard]] rc::RobotDoubleTouchedBall robotDoubleTouchedBallFromAttackerDoubleTouchedBall(
      const tp::AttackerDoubleTouchedBall& attacker_double_touched_ball) const {
    rc::RobotDoubleTouchedBall result;

    result.set_by_team(teamFromTeam(attacker_double_touched_ball.by_team()));
    *result.mutable_by_robot() = robotIdFromTeamAndNumber(attacker_double_touched_ball.by_team(),
                                                          attacker_double_touched_ball.by_bot());
    *result.mutable_ball_position() = point2DfFromVector2(attacker_double_touched_ball.location());

    return result;
  }

  [[nodiscard]] rc::NoProgressInGame noProgressInGameFromNoProgressInGame( // NOLINT(*static*)
      const tp::NoProgressInGame& no_progress_in_game) const {
    rc::NoProgressInGame result;

    *result.mutable_ball_position() = point2DfFromVector2(no_progress_in_game.location());
    *result.mutable_duration() = durationFromSeconds(no_progress_in_game.time());

    return result;
  }

  [[nodiscard]] rc::MultipleCards
  multipleCardsFromMultipleCards(const tp::MultipleCards& multiple_cards) const {
    rc::MultipleCards result;

    result.set_by_team(teamFromTeam(multiple_cards.by_team()));

    return result;
  }

  [[nodiscard]] rc::MultipleFouls
  multipleFoulsFromMultipleFouls(const tp::MultipleFouls& multiple_fouls) const {
    rc::MultipleFouls result;

    result.set_by_team(teamFromTeam(multiple_fouls.by_team()));

    return result;
  }

  [[nodiscard]] rc::TooManyRobots
  tooManyRobotsFromTooManyRobots(const tp::TooManyRobots& too_many_robots) const {
    rc::TooManyRobots result;

    result.set_by_team(teamFromTeam(too_many_robots.by_team()));
    result.set_num_robots_allowed(too_many_robots.num_robots_allowed());
    result.set_num_robots_on_field(too_many_robots.num_robots_on_field());
    *result.mutable_ball_position() = point2DfFromVector2(too_many_robots.ball_location());

    return result;
  }

  [[nodiscard]] rc::BallPlacementSucceeded ballPlacementSucceededFromPlacementSucceeded(
      const tp::PlacementSucceeded& placement_succeeded) const {
    rc::BallPlacementSucceeded result;

    result.set_by_team(teamFromTeam(placement_succeeded.by_team()));
    *result.mutable_duration() = durationFromSeconds(placement_succeeded.time_taken());
    result.set_distance_to_placement_target(placement_succeeded.precision()
                                            * kFMillimetersPerMeters);
    result.set_distance_from_start_position(placement_succeeded.distance()
                                            * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::BallPlacementFailed
  ballPlacementFailedFromPlacementFailed(const tp::PlacementFailed& placement_failed) const {
    rc::BallPlacementFailed result;

    result.set_by_team(teamFromTeam(placement_failed.by_team()));
    result.set_remaining_distance(placement_failed.remaining_distance() * kFMillimetersPerMeters);

    return result;
  }

  [[nodiscard]] rc::PenaltyKickFailed
  penaltyKickFailedFromPenaltyKickFailed(const tp::PenaltyKickFailed& penalty_kick_failed) const {
    rc::PenaltyKickFailed result;

    result.set_by_team(teamFromTeam(penalty_kick_failed.by_team()));
    *result.mutable_ball_position() = point2DfFromVector2(penalty_kick_failed.location());

    return result;
  }

  [[nodiscard]] rc::Goal goalFromGoal(const tp::Goal& goal) const {
    rc::Goal result;

    result.set_by_team(teamFromTeam(goal.by_team()));
    result.set_kicking_team(teamFromTeam(goal.kicking_team()));
    *result.mutable_kicking_robot()
        = robotIdFromTeamAndNumber(goal.kicking_team(), goal.kicking_bot());
    *result.mutable_ball_position_when_scored() = point2DfFromVector2(goal.location());
    *result.mutable_kick_position() = point2DfFromVector2(goal.kick_location());
    result.set_max_ball_height(goal.max_ball_height() * kFMillimetersPerMeters);
    result.set_num_of_robots_of_scoring_team(goal.num_robots_by_team());
    *result.mutable_last_touch_by_scoring_team_timestamp()
        = timestampFromUnixNanos(goal.last_touch_by_team());
    result.set_message(goal.message());

    return result;
  }

  [[nodiscard]] rc::RobotSubstitution
  robotSubstitutionFromBotSubstitution(const tp::BotSubstitution& bot_substitution) const {
    rc::RobotSubstitution result;

    result.set_by_team(teamFromTeam(bot_substitution.by_team()));

    return result;
  }

  [[nodiscard]] rc::ChallengeFlag
  challengeFlagFromChallengeFlag(const tp::ChallengeFlag& challenge_flag) const {
    rc::ChallengeFlag result;

    result.set_by_team(teamFromTeam(challenge_flag.by_team()));

    return result;
  }

  [[nodiscard]] rc::EmergencyStop
  emergencyStopFromEmergencyStop(const tp::EmergencyStop& emergency_stop) const {
    rc::EmergencyStop result;

    result.set_by_team(teamFromTeam(emergency_stop.by_team()));

    return result;
  }

  [[nodiscard]] rc::UnsportingBehaviorMinor unsportingBehaviorMinorFromUnsportingBehaviorMinor(
      const tp::UnsportingBehaviorMinor& unsporting_behavior_minor) const {
    rc::UnsportingBehaviorMinor result;

    result.set_by_team(teamFromTeam(unsporting_behavior_minor.by_team()));
    result.set_reason(unsporting_behavior_minor.reason());

    return result;
  }

  [[nodiscard]] rc::UnsportingBehaviorMajor unsportingBehaviorMajorFromUnsportingBehaviorMajor(
      const tp::UnsportingBehaviorMajor& unsporting_behavior_major) const {
    rc::UnsportingBehaviorMajor result;

    result.set_by_team(teamFromTeam(unsporting_behavior_major.by_team()));
    result.set_reason(unsporting_behavior_major.reason());

    return result;
  }

 private:
  static rc::RobotId robotIdFromTeamAndNumber(tp::Team team, uint32_t number) {
    rc::RobotId result;

    result.set_color(robotIdColorFromTeam(team));
    result.set_number(number);

    return result;
  }

  static rc::Point2Df point2DfFromVector2(const tp::Vector2& vector2) {
    rc::Point2Df result;

    result.set_x(vector2.x() * kFMillimetersPerMeters);
    result.set_y(vector2.y() * kFMillimetersPerMeters);

    return result;
  }

  static ::google::protobuf::Duration durationFromSeconds(float seconds) {
    return TimeUtil::SecondsToDuration(static_cast<int64_t>(seconds));
  }

  static ::google::protobuf::Timestamp timestampFromUnixNanos(uint64_t nanoseconds) {
    return TimeUtil::NanosecondsToTimestamp(static_cast<int64_t>(nanoseconds));
  }

  [[nodiscard]] rc::Team teamFromTeam(tp::Team by_team) const {
    switch (by_team) {
      case tp::Team::BLUE: return home_is_blue_team_ ? rc::Team::TEAM_HOME : rc::Team::TEAM_AWAY;
      case tp::Team::YELLOW: return home_is_blue_team_ ? rc::Team::TEAM_AWAY : rc::Team::TEAM_HOME;
      default: return rc::Team::TEAM_UNSPECIFIED;
    }
    std::unreachable();
  }

  static rc::RobotId::Color robotIdColorFromTeam(tp::Team team) {
    switch (team) {
      case tp::Team::BLUE: return rc::RobotId::COLOR_BLUE;
      case tp::Team::YELLOW: return rc::RobotId::COLOR_YELLOW;
      default: return rc::RobotId::COLOR_UNSPECIFIED;
    }
    std::unreachable();
  }

  bool home_is_blue_team_ = false;
};

rc::GameEvent fromGameControllerEvent(const detection_util::Timestamp& now,
                                      const tp::GameEvent& game_event,
                                      bool home_is_blue_team) {
  using enum tp::GameEvent::EventCase;

  MapperInternal mapper{
      home_is_blue_team,
  };

  rc::GameEvent result;
  *result.mutable_sources() = game_event.origin();

  /* set timestamp from detection clock */ {
    ::google::protobuf::Timestamp timestamp;
    timestamp.set_seconds(now.seconds());
    timestamp.set_nanos(now.nanos());

    *result.mutable_timestamp() = timestamp;
  }

  switch (game_event.event_case()) {
    case kBallLeftFieldTouchLine: {
      *result.mutable_ball_left_field_touch_line()
          = mapper.ballLeftFieldFromBallLeftField(game_event.ball_left_field_touch_line());
      break;
    }
    case kBallLeftFieldGoalLine: {
      *result.mutable_ball_left_field_goal_line()
          = mapper.ballLeftFieldFromBallLeftField(game_event.ball_left_field_goal_line());
      break;
    }
    case kBoundaryCrossing: {
      *result.mutable_ball_left_field_boundary()
          = mapper.ballLeftFieldBoundaryFromBoundaryCrossing(game_event.boundary_crossing());
      break;
    }
    case kAimlessKick: {
      *result.mutable_aimless_kick() = mapper.aimlessKickFromAimlessKick(game_event.aimless_kick());
      break;
    }
    case kKeeperHeldBall: {
      *result.mutable_goalkeeper_held_ball()
          = mapper.goalkeeperHeldBallFromKeeperHeldBall(game_event.keeper_held_ball());
      break;
    }
    case kAttackerTooCloseToDefenseArea: {
      *result.mutable_robot_too_close_to_defense_area()
          = mapper.robotTooCloseToDefenseAreaFromAttackerTooCloseToDefenseArea(
              game_event.attacker_too_close_to_defense_area());
      break;
    }
    case kDefenderInDefenseArea: {
      *result.mutable_robot_in_defense_area() = mapper.robotInDefenseAreaFromDefenderInDefenseArea(
          game_event.defender_in_defense_area());
      break;
    }
    case kBotPushedBot: {
      *result.mutable_robot_pushed_robot()
          = mapper.robotPushedRobotFromBotPushedBot(game_event.bot_pushed_bot());
      break;
    }
    case kBotHeldBallDeliberately: {
      *result.mutable_robot_held_ball_deliberately()
          = mapper.robotHeldBallDeliberatelyFromBotHeldBallDeliberately(
              game_event.bot_held_ball_deliberately());
      break;
    }
    case kBotDribbledBallTooFar: {
      *result.mutable_robot_dribbled_ball_too_far()
          = mapper.robotDribbledBallTooFarFromBotDribbledBallTooFar(
              game_event.bot_dribbled_ball_too_far());
      break;
    }
    case kBotTippedOver: {
      *result.mutable_robot_tipped_over()
          = mapper.robotTippedOverFromBotTippedOver(game_event.bot_tipped_over());
      break;
    }
    case kAttackerTouchedBallInDefenseArea: {
      *result.mutable_robot_touched_ball_in_defense_area()
          = mapper.robotTouchedBallInDefenseAreaFromAttackerTouchedBallInDefenseArea(
              game_event.attacker_touched_ball_in_defense_area());
      break;
    }
    case kBotKickedBallTooFast: {
      *result.mutable_robot_kicked_ball_too_fast()
          = mapper.robotKickedBallTooFastFromBotKickedBallTooFast(
              game_event.bot_kicked_ball_too_fast());
      break;
    }
    case kBotCrashUnique: {
      *result.mutable_robot_crash_unique()
          = mapper.robotCrashUniqueFromBotCrashUnique(game_event.bot_crash_unique());
      break;
    }
    case kBotCrashDrawn: {
      *result.mutable_robot_crash_drawn()
          = mapper.robotCrashDrawnFromBotCrashDrawn(game_event.bot_crash_drawn());
      break;
    }
    case kBotTooFastInStop: {
      *result.mutable_robot_too_fast_in_stop()
          = mapper.robotTooFastInStopFromBotTooFastInStop(game_event.bot_too_fast_in_stop());
      break;
    }
    case kDefenderTooCloseToKickPoint: {
      *result.mutable_robot_too_close_to_kick_point()
          = mapper.robotTooCloseToKickPointFromDefenderTooCloseToKickPoint(
              game_event.defender_too_close_to_kick_point());
      break;
    }
    case kBotInterferedPlacement: {
      *result.mutable_robot_interfered_ball_placement()
          = mapper.robotInterferedBallPlacementFromBotInterferedPlacement(
              game_event.bot_interfered_placement());
      break;
    }
    case kAttackerDoubleTouchedBall: {
      *result.mutable_robot_double_touched_ball()
          = mapper.robotDoubleTouchedBallFromAttackerDoubleTouchedBall(
              game_event.attacker_double_touched_ball());
      break;
    }
    case kNoProgressInGame: {
      *result.mutable_no_progress_in_game()
          = mapper.noProgressInGameFromNoProgressInGame(game_event.no_progress_in_game());
      break;
    }
    case kMultipleCards: {
      *result.mutable_multiple_cards()
          = mapper.multipleCardsFromMultipleCards(game_event.multiple_cards());
      break;
    }
    case kMultipleFouls: {
      *result.mutable_multiple_fouls()
          = mapper.multipleFoulsFromMultipleFouls(game_event.multiple_fouls());
      break;
    }
    case kTooManyRobots: {
      *result.mutable_too_many_robots()
          = mapper.tooManyRobotsFromTooManyRobots(game_event.too_many_robots());
      break;
    }
    case kPlacementSucceeded: {
      *result.mutable_ball_placement_succeeded()
          = mapper.ballPlacementSucceededFromPlacementSucceeded(game_event.placement_succeeded());
      break;
    }
    case kPlacementFailed: {
      *result.mutable_ball_placement_failed()
          = mapper.ballPlacementFailedFromPlacementFailed(game_event.placement_failed());
      break;
    }
    case kPenaltyKickFailed: {
      *result.mutable_penalty_kick_failed()
          = mapper.penaltyKickFailedFromPenaltyKickFailed(game_event.penalty_kick_failed());
      break;
    }
    case kPossibleGoal: {
      *result.mutable_possible_goal() = mapper.goalFromGoal(game_event.possible_goal());
      break;
    }
    case kGoal: {
      *result.mutable_goal() = mapper.goalFromGoal(game_event.goal());
      break;
    }
    case kInvalidGoal: {
      *result.mutable_invalid_goal() = mapper.goalFromGoal(game_event.invalid_goal());
      break;
    }
    case kBotSubstitution: {
      *result.mutable_robot_substitution()
          = mapper.robotSubstitutionFromBotSubstitution(game_event.bot_substitution());
      break;
    }
    case kChallengeFlag: {
      *result.mutable_challenge_flag()
          = mapper.challengeFlagFromChallengeFlag(game_event.challenge_flag());
      break;
    }
    case kEmergencyStop: {
      *result.mutable_emergency_stop()
          = mapper.emergencyStopFromEmergencyStop(game_event.emergency_stop());
      break;
    }
    case kUnsportingBehaviorMinor: {
      *result.mutable_unsporting_behavior_minor()
          = mapper.unsportingBehaviorMinorFromUnsportingBehaviorMinor(
              game_event.unsporting_behavior_minor());
      break;
    }
    case kUnsportingBehaviorMajor: {
      *result.mutable_unsporting_behavior_major()
          = mapper.unsportingBehaviorMajorFromUnsportingBehaviorMajor(
              game_event.unsporting_behavior_major());
      break;
    }
    case /* deprecated */ kPrepared: [[fallthrough]];
    case /* deprecated */ kIndirectGoal: [[fallthrough]];
    case /* deprecated */ kChippedGoal: [[fallthrough]];
    case /* deprecated */ kKickTimeout: [[fallthrough]];
    case /* deprecated */ kAttackerTouchedOpponentInDefenseArea: [[fallthrough]];
    case /* deprecated */ kAttackerTouchedOpponentInDefenseAreaSkipped: [[fallthrough]];
    case /* deprecated */ kBotCrashUniqueSkipped: [[fallthrough]];
    case /* deprecated */ kBotPushedBotSkipped: [[fallthrough]];
    case /* deprecated */ kDefenderInDefenseAreaPartially: [[fallthrough]];
    case /* deprecated */ kMultiplePlacementFailures: [[fallthrough]];
    case /* do nothing */ EVENT_NOT_SET: break;
  }

  return result;
}

} // namespace

RepeatedPtrField<rc::GameEvent> GameEventsMapper::fromReferee(const tp::Referee& referee) {
  RepeatedPtrField<rc::GameEvent> result;

  // all events are cleared as soon as the game resumes.
  if (referee.game_events().empty()) {
    if (!game_events_map_.empty()) {
      game_events_map_.clear();
    }
    return result;
  }

  bool home_is_blue_team = !referee.blue_team_on_positive_half();
  detection_util::Timestamp now = detection_util::Clock::now();

  result.Reserve(static_cast<int>(referee.game_events_size()));
  for (const tp::GameEvent& tp_game_event : referee.game_events()) {
    std::string serialized_tp_event = tp_game_event.SerializeAsString();
    auto it = game_events_map_.find(serialized_tp_event);

    if (it == game_events_map_.end()) {
      std::tie(it, std::ignore) = game_events_map_.emplace(
          serialized_tp_event,
          fromGameControllerEvent(now, tp_game_event, home_is_blue_team));
    }

    *result.Add() = it->second;
  }

  return result;
}

RepeatedPtrField<rc::GameEventsProposal>
GameEventsMapper::proposalsFromReferee(const tp::Referee& referee) {
  RepeatedPtrField<rc::GameEventsProposal> result;

  // all events are cleared as soon as the game resumes.
  if (referee.game_events().empty()) {
    if (!game_events_proposal_map_.empty()) {
      game_events_proposal_map_.clear();
    }
    return result;
  }

  bool home_is_blue_team = !referee.blue_team_on_positive_half();
  detection_util::Timestamp now = detection_util::Clock::now();

  result.Reserve(static_cast<int>(referee.game_event_proposals_size()));
  for (const tp::GameEventProposalGroup& tp_proposal_group : referee.game_event_proposals()) {
    object_ptr rc_proposal_group = result.Add();
    rc_proposal_group->set_proposal_id(tp_proposal_group.id());
    rc_proposal_group->set_was_accepted(tp_proposal_group.accepted());

    rc_proposal_group->mutable_game_events()->Reserve(tp_proposal_group.game_events_size());
    for (const tp::GameEvent& tp_game_event : tp_proposal_group.game_events()) {
      std::string serialized_tp_event = tp_game_event.SerializeAsString();
      auto it = game_events_proposal_map_.find(serialized_tp_event);

      if (it == game_events_proposal_map_.end()) {
        std::tie(it, std::ignore) = game_events_proposal_map_.emplace(
            serialized_tp_event,
            fromGameControllerEvent(now, tp_game_event, home_is_blue_team));
      }

      *rc_proposal_group->mutable_game_events()->Add() = it->second;
    }
  }

  return result;
}

} // namespace referee

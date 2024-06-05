#include "referee/game_event/game_event_mapper.h"

#include <google/protobuf/arena.h>
#include <google/protobuf/duration.pb.h>
#include <google/protobuf/timestamp.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>

namespace referee {
namespace {

using ::google::protobuf::Arena;
using ::robocin::observer_ptr;

constexpr float kFMetersToMillimeters = 1e3F;
constexpr float kFSecondsToNanoseconds = 1e9F;
constexpr uint64_t kULLSecondsToNanoseconds = 1'000'000'000ULL;

namespace rc {

using GameEvent = ::protocols::common::GameEvent;

using Team = GameEvent::Team;
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

using GameEvent = ::protocols::third_party::game_controller::GameEvent;

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
  MapperInternal(bool home_is_blue_team, observer_ptr<Arena> arena) :
      home_is_blue_team_(home_is_blue_team),
      arena_(arena) {}

  observer_ptr<rc::BallLeftField>
  ballLeftFieldFromBallLeftField(const tp::BallLeftField& ball_left_field) {
    observer_ptr result = Arena::CreateMessage<rc::BallLeftField>(arena_.get());

    result->set_by_team(teamFromTeam(ball_left_field.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(ball_left_field.by_team(), ball_left_field.by_bot()).get());
    result->set_allocated_left_field_position(
        point2DfFromVector2(ball_left_field.location()).get());

    return result;
  }

  observer_ptr<rc::BallLeftFieldBoundary>
  ballLeftFieldBoundaryFromBoundaryCrossing(const tp::BoundaryCrossing& boundary_crossing) {
    observer_ptr result = Arena::CreateMessage<rc::BallLeftFieldBoundary>(arena_.get());

    result->set_by_team(teamFromTeam(boundary_crossing.by_team()));
    result->set_allocated_left_field_boundary_position(
        point2DfFromVector2(boundary_crossing.location()).get());

    return result;
  }

  observer_ptr<rc::AimlessKick> aimlessKickFromAimlessKick(const tp::AimlessKick& aimless_kick) {
    observer_ptr result = Arena::CreateMessage<rc::AimlessKick>(arena_.get());

    result->set_by_team(teamFromTeam(aimless_kick.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(aimless_kick.by_team(), aimless_kick.by_bot()).get());
    result->set_allocated_left_field_position(point2DfFromVector2(aimless_kick.location()).get());
    result->set_allocated_kick_position(point2DfFromVector2(aimless_kick.kick_location()).get());

    return result;
  }

  observer_ptr<rc::GoalkeeperHeldBall>
  goalkeeperHeldBallFromKeeperHeldBall(const tp::KeeperHeldBall& keeper_held_ball) {
    observer_ptr result = Arena::CreateMessage<rc::GoalkeeperHeldBall>(arena_.get());

    result->set_by_team(teamFromTeam(keeper_held_ball.by_team()));
    result->set_allocated_ball_position(point2DfFromVector2(keeper_held_ball.location()).get());
    result->set_allocated_duration(durationFromSeconds(keeper_held_ball.duration()).get());

    return result;
  }

  observer_ptr<rc::RobotTooCloseToDefenseArea>
  robotTooCloseToDefenseAreaFromAttackerTooCloseToDefenseArea(
      const tp::AttackerTooCloseToDefenseArea& attacker_too_close_to_defense_area) {
    observer_ptr result = Arena::CreateMessage<rc::RobotTooCloseToDefenseArea>(arena_.get());

    result->set_by_team(teamFromTeam(attacker_too_close_to_defense_area.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(attacker_too_close_to_defense_area.by_team(),
                                 attacker_too_close_to_defense_area.by_bot())
            .get());
    result->set_allocated_robot_position(
        point2DfFromVector2(attacker_too_close_to_defense_area.location()).get());
    result->set_distance_to_defense_area(attacker_too_close_to_defense_area.distance()
                                         * kFMetersToMillimeters);
    result->set_allocated_ball_position(
        point2DfFromVector2(attacker_too_close_to_defense_area.ball_location()).get());

    return result;
  }

  observer_ptr<rc::RobotInDefenseArea> robotInDefenseAreaFromDefenderInDefenseArea(
      const tp::DefenderInDefenseArea& defender_in_defense_area) {
    observer_ptr result = Arena::CreateMessage<rc::RobotInDefenseArea>(arena_.get());

    result->set_by_team(teamFromTeam(defender_in_defense_area.by_team()));
    result->set_allocated_by_robot(robotIdFromTeamAndNumber(defender_in_defense_area.by_team(),
                                                            defender_in_defense_area.by_bot())
                                       .get());
    result->set_allocated_robot_position(
        point2DfFromVector2(defender_in_defense_area.location()).get());
    result->set_distance_to_nearest_point_outside_area(defender_in_defense_area.distance()
                                                       * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::RobotPushedRobot>
  robotPushedRobotFromBotPushedBot(const tp::BotPushedBot& bot_pushed_bot) {
    observer_ptr result = Arena::CreateMessage<rc::RobotPushedRobot>(arena_.get());

    result->set_by_team(teamFromTeam(bot_pushed_bot.by_team()));
    result->set_allocated_violator_robot(
        robotIdFromTeamAndNumber(bot_pushed_bot.by_team(), bot_pushed_bot.violator()).get());
    result->set_allocated_victim_robot(
        robotIdFromTeamAndNumber(tp::otherTeam(bot_pushed_bot.by_team()), bot_pushed_bot.victim())
            .get());
    result->set_allocated_position(point2DfFromVector2(bot_pushed_bot.location()).get());
    result->set_pushed_distance(bot_pushed_bot.pushed_distance() * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::RobotHeldBallDeliberately> robotHeldBallDeliberatelyFromBotHeldBallDeliberately(
      const tp::BotHeldBallDeliberately& bot_held_ball_deliberately) {
    observer_ptr result = Arena::CreateMessage<rc::RobotHeldBallDeliberately>(arena_.get());

    result->set_by_team(teamFromTeam(bot_held_ball_deliberately.by_team()));
    result->set_allocated_by_robot(robotIdFromTeamAndNumber(bot_held_ball_deliberately.by_team(),
                                                            bot_held_ball_deliberately.by_bot())
                                       .get());
    result->set_allocated_ball_position(
        point2DfFromVector2(bot_held_ball_deliberately.location()).get());
    result->set_allocated_duration(
        durationFromSeconds(bot_held_ball_deliberately.duration()).get());

    return result;
  }

  observer_ptr<rc::RobotDribbledBallTooFar> robotDribbledBallTooFarFromBotDribbledBallTooFar(
      const tp::BotDribbledBallTooFar& bot_dribbled_ball_too_far) {
    observer_ptr result = Arena::CreateMessage<rc::RobotDribbledBallTooFar>(arena_.get());

    result->set_by_team(teamFromTeam(bot_dribbled_ball_too_far.by_team()));
    result->set_allocated_by_robot(robotIdFromTeamAndNumber(bot_dribbled_ball_too_far.by_team(),
                                                            bot_dribbled_ball_too_far.by_bot())
                                       .get());
    result->set_allocated_start_position(
        point2DfFromVector2(bot_dribbled_ball_too_far.start()).get());
    result->set_allocated_end_position(point2DfFromVector2(bot_dribbled_ball_too_far.end()).get());

    return result;
  }

  observer_ptr<rc::RobotTippedOver>
  robotTippedOverFromBotTippedOver(const tp::BotTippedOver& bot_tipped_over) {
    observer_ptr result = Arena::CreateMessage<rc::RobotTippedOver>(arena_.get());

    result->set_by_team(teamFromTeam(bot_tipped_over.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(bot_tipped_over.by_team(), bot_tipped_over.by_bot()).get());
    result->set_allocated_robot_position(point2DfFromVector2(bot_tipped_over.location()).get());
    result->set_allocated_ball_position(point2DfFromVector2(bot_tipped_over.ball_location()).get());

    return result;
  }

  observer_ptr<rc::RobotTouchedBallInDefenseArea>
  robotTouchedBallInDefenseAreaFromAttackerTouchedBallInDefenseArea(
      const tp::AttackerTouchedBallInDefenseArea& attacker_touched_ball_in_defense_area) {
    observer_ptr result = Arena::CreateMessage<rc::RobotTouchedBallInDefenseArea>(arena_.get());

    result->set_by_team(teamFromTeam(attacker_touched_ball_in_defense_area.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(attacker_touched_ball_in_defense_area.by_team(),
                                 attacker_touched_ball_in_defense_area.by_bot())
            .get());
    result->set_allocated_robot_position(
        point2DfFromVector2(attacker_touched_ball_in_defense_area.location()).get());
    result->set_distance_to_nearest_point_outside_area(
        attacker_touched_ball_in_defense_area.distance() * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::RobotKickedBallTooFast> robotKickedBallTooFastFromBotKickedBallTooFast(
      const tp::BotKickedBallTooFast& bot_kicked_ball_too_fast) {
    observer_ptr result = Arena::CreateMessage<rc::RobotKickedBallTooFast>(arena_.get());

    result->set_by_team(teamFromTeam(bot_kicked_ball_too_fast.by_team()));
    result->set_allocated_by_robot(robotIdFromTeamAndNumber(bot_kicked_ball_too_fast.by_team(),
                                                            bot_kicked_ball_too_fast.by_bot())
                                       .get());
    result->set_allocated_ball_at_highest_speed_position(
        point2DfFromVector2(bot_kicked_ball_too_fast.location()).get());
    result->set_initial_ball_speed(bot_kicked_ball_too_fast.initial_ball_speed()
                                   * kFMetersToMillimeters);
    result->set_was_chipped_kick(bot_kicked_ball_too_fast.chipped());

    return result;
  }

  observer_ptr<rc::RobotCrashUnique>
  robotCrashUniqueFromBotCrashUnique(const tp::BotCrashUnique& bot_crash_unique) {
    observer_ptr result = Arena::CreateMessage<rc::RobotCrashUnique>(arena_.get());

    result->set_by_team(teamFromTeam(bot_crash_unique.by_team()));
    result->set_allocated_violator_robot(
        robotIdFromTeamAndNumber(bot_crash_unique.by_team(), bot_crash_unique.violator()).get());
    result->set_allocated_victim_robot(
        robotIdFromTeamAndNumber(tp::otherTeam(bot_crash_unique.by_team()),
                                 bot_crash_unique.victim())
            .get());
    result->set_allocated_crash_position(point2DfFromVector2(bot_crash_unique.location()).get());
    result->set_crash_speed(bot_crash_unique.crash_speed() * kFMetersToMillimeters);
    result->set_speed_difference(bot_crash_unique.speed_diff() * kFMetersToMillimeters);
    result->set_crash_angle(bot_crash_unique.crash_angle());

    return result;
  }

  observer_ptr<rc::RobotCrashDrawn>
  robotCrashDrawnFromBotCrashDrawn(const tp::BotCrashDrawn& bot_crash_drawn) {
    observer_ptr result = Arena::CreateMessage<rc::RobotCrashDrawn>(arena_.get());

    if (home_is_blue_team_) {
      result->set_allocated_home_robot_id(
          robotIdFromTeamAndNumber(tp::Team::BLUE, bot_crash_drawn.bot_blue()).get());
      result->set_allocated_away_robot_id(
          robotIdFromTeamAndNumber(tp::Team::YELLOW, bot_crash_drawn.bot_yellow()).get());
    } else {
      result->set_allocated_home_robot_id(
          robotIdFromTeamAndNumber(tp::Team::YELLOW, bot_crash_drawn.bot_yellow()).get());
      result->set_allocated_away_robot_id(
          robotIdFromTeamAndNumber(tp::Team::BLUE, bot_crash_drawn.bot_blue()).get());
    }

    result->set_allocated_crash_position(point2DfFromVector2(bot_crash_drawn.location()).get());
    result->set_crash_speed(bot_crash_drawn.crash_speed() * kFMetersToMillimeters);
    result->set_speed_difference(bot_crash_drawn.speed_diff() * kFMetersToMillimeters);
    result->set_crash_angle(bot_crash_drawn.crash_angle());

    return result;
  }

  observer_ptr<rc::RobotTooFastInStop>
  robotTooFastInStopFromBotTooFastInStop(const tp::BotTooFastInStop& bot_too_fast_in_stop) {
    observer_ptr result = Arena::CreateMessage<rc::RobotTooFastInStop>(arena_.get());

    result->set_by_team(teamFromTeam(bot_too_fast_in_stop.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(bot_too_fast_in_stop.by_team(), bot_too_fast_in_stop.by_bot())
            .get());
    result->set_allocated_robot_position(
        point2DfFromVector2(bot_too_fast_in_stop.location()).get());
    result->set_robot_speed(bot_too_fast_in_stop.speed() * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::RobotTooCloseToKickPoint>
  robotTooCloseToKickPointFromDefenderTooCloseToKickPoint(
      const tp::DefenderTooCloseToKickPoint& defender_too_close_to_kick_point) {
    observer_ptr result = Arena::CreateMessage<rc::RobotTooCloseToKickPoint>(arena_.get());

    result->set_by_team(teamFromTeam(defender_too_close_to_kick_point.by_team()));
    result->set_allocated_by_robot(
        robotIdFromTeamAndNumber(defender_too_close_to_kick_point.by_team(),
                                 defender_too_close_to_kick_point.by_bot())
            .get());
    result->set_allocated_robot_position(
        point2DfFromVector2(defender_too_close_to_kick_point.location()).get());
    result->set_robot_distance(defender_too_close_to_kick_point.distance() * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::RobotInterferedBallPlacement>
  robotInterferedBallPlacementFromBotInterferedPlacement(
      const tp::BotInterferedPlacement& bot_interfered_placement) {
    observer_ptr result = Arena::CreateMessage<rc::RobotInterferedBallPlacement>(arena_.get());

    result->set_by_team(teamFromTeam(bot_interfered_placement.by_team()));
    result->set_allocated_by_robot(robotIdFromTeamAndNumber(bot_interfered_placement.by_team(),
                                                            bot_interfered_placement.by_bot())
                                       .get());
    result->set_allocated_robot_position(
        point2DfFromVector2(bot_interfered_placement.location()).get());

    return result;
  }

  observer_ptr<rc::RobotDoubleTouchedBall> robotDoubleTouchedBallFromAttackerDoubleTouchedBall(
      const tp::AttackerDoubleTouchedBall& attacker_double_touched_ball) {
    observer_ptr result = Arena::CreateMessage<rc::RobotDoubleTouchedBall>(arena_.get());

    result->set_by_team(teamFromTeam(attacker_double_touched_ball.by_team()));
    result->set_allocated_by_robot(robotIdFromTeamAndNumber(attacker_double_touched_ball.by_team(),
                                                            attacker_double_touched_ball.by_bot())
                                       .get());
    result->set_allocated_ball_position(
        point2DfFromVector2(attacker_double_touched_ball.location()).get());

    return result;
  }

  observer_ptr<rc::NoProgressInGame>
  noProgressInGameFromNoProgressInGame(const tp::NoProgressInGame& no_progress_in_game) {
    observer_ptr result = Arena::CreateMessage<rc::NoProgressInGame>(arena_.get());

    result->set_allocated_ball_position(point2DfFromVector2(no_progress_in_game.location()).get());
    result->set_allocated_duration(durationFromSeconds(no_progress_in_game.time()).get());

    return result;
  }

  observer_ptr<rc::MultipleCards>
  multipleCardsFromMultipleCards(const tp::MultipleCards& multiple_cards) {
    observer_ptr result = Arena::CreateMessage<rc::MultipleCards>(arena_.get());

    result->set_by_team(teamFromTeam(multiple_cards.by_team()));

    return result;
  }

  observer_ptr<rc::MultipleFouls>
  multipleFoulsFromMultipleFouls(const tp::MultipleFouls& multiple_fouls) {
    observer_ptr result = Arena::CreateMessage<rc::MultipleFouls>(arena_.get());

    result->set_by_team(teamFromTeam(multiple_fouls.by_team()));

    return result;
  }

  observer_ptr<rc::TooManyRobots>
  tooManyRobotsFromTooManyRobots(const tp::TooManyRobots& too_many_robots) {
    observer_ptr result = Arena::CreateMessage<rc::TooManyRobots>(arena_.get());

    result->set_by_team(teamFromTeam(too_many_robots.by_team()));
    result->set_num_robots_allowed(too_many_robots.num_robots_allowed());
    result->set_num_robots_on_field(too_many_robots.num_robots_on_field());
    result->set_allocated_ball_position(point2DfFromVector2(too_many_robots.ball_location()).get());

    return result;
  }

  observer_ptr<rc::BallPlacementSucceeded>
  ballPlacementSucceededFromPlacementSucceeded(const tp::PlacementSucceeded& placement_succeeded) {
    observer_ptr result = Arena::CreateMessage<rc::BallPlacementSucceeded>(arena_.get());

    result->set_by_team(teamFromTeam(placement_succeeded.by_team()));
    result->set_allocated_duration(durationFromSeconds(placement_succeeded.time_taken()).get());
    result->set_distance_to_placement_target(placement_succeeded.precision()
                                             * kFMetersToMillimeters);
    result->set_distance_from_start_position(placement_succeeded.distance()
                                             * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::BallPlacementFailed>
  ballPlacementFailedFromPlacementFailed(const tp::PlacementFailed& placement_failed) {
    observer_ptr result = Arena::CreateMessage<rc::BallPlacementFailed>(arena_.get());

    result->set_by_team(teamFromTeam(placement_failed.by_team()));
    result->set_remaining_distance(placement_failed.remaining_distance() * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<rc::PenaltyKickFailed>
  penaltyKickFailedFromPenaltyKickFailed(const tp::PenaltyKickFailed& penalty_kick_failed) {
    observer_ptr result = Arena::CreateMessage<rc::PenaltyKickFailed>(arena_.get());

    result->set_by_team(teamFromTeam(penalty_kick_failed.by_team()));
    result->set_allocated_ball_position(point2DfFromVector2(penalty_kick_failed.location()).get());

    return result;
  }

  observer_ptr<rc::Goal> goalFromGoal(const tp::Goal& goal) {
    observer_ptr result = Arena::CreateMessage<rc::Goal>(arena_.get());

    result->set_by_team(teamFromTeam(goal.by_team()));
    result->set_kicking_team(teamFromTeam(goal.kicking_team()));
    result->set_allocated_kicking_robot(
        robotIdFromTeamAndNumber(goal.kicking_team(), goal.kicking_bot()).get());
    result->set_allocated_ball_position_when_scored(point2DfFromVector2(goal.location()).get());
    result->set_allocated_kick_position(point2DfFromVector2(goal.kick_location()).get());
    result->set_max_ball_height(goal.max_ball_height() * kFMetersToMillimeters);
    result->set_num_of_robots_of_scoring_team(goal.num_robots_by_team());
    result->set_allocated_last_touch_by_scoring_team_timestamp(
        timestampFromUnixNanos(goal.last_touch_by_team()).get());
    result->set_message(goal.message());

    return result;
  }

  observer_ptr<rc::RobotSubstitution>
  robotSubstitutionFromBotSubstitution(const tp::BotSubstitution& bot_substitution) {
    observer_ptr result = Arena::CreateMessage<rc::RobotSubstitution>(arena_.get());

    result->set_by_team(teamFromTeam(bot_substitution.by_team()));

    return result;
  }

  observer_ptr<rc::ChallengeFlag>
  challengeFlagFromChallengeFlag(const tp::ChallengeFlag& challenge_flag) {
    observer_ptr result = Arena::CreateMessage<rc::ChallengeFlag>(arena_.get());

    result->set_by_team(teamFromTeam(challenge_flag.by_team()));

    return result;
  }

  observer_ptr<rc::EmergencyStop>
  emergencyStopFromEmergencyStop(const tp::EmergencyStop& emergency_stop) {
    observer_ptr result = Arena::CreateMessage<rc::EmergencyStop>(arena_.get());

    result->set_by_team(teamFromTeam(emergency_stop.by_team()));

    return result;
  }

  observer_ptr<rc::UnsportingBehaviorMinor> unsportingBehaviorMinorFromUnsportingBehaviorMinor(
      const tp::UnsportingBehaviorMinor& unsporting_behavior_minor) {
    observer_ptr result = Arena::CreateMessage<rc::UnsportingBehaviorMinor>(arena_.get());

    result->set_by_team(teamFromTeam(unsporting_behavior_minor.by_team()));
    result->set_reason(unsporting_behavior_minor.reason());

    return result;
  }

  observer_ptr<rc::UnsportingBehaviorMajor> unsportingBehaviorMajorFromUnsportingBehaviorMajor(
      const tp::UnsportingBehaviorMajor& unsporting_behavior_major) {
    observer_ptr result = Arena::CreateMessage<rc::UnsportingBehaviorMajor>(arena_.get());

    result->set_by_team(teamFromTeam(unsporting_behavior_major.by_team()));
    result->set_reason(unsporting_behavior_major.reason());

    return result;
  }

 private:
  observer_ptr<rc::RobotId> robotIdFromTeamAndNumber(tp::Team team, uint32_t number) {
    observer_ptr result = Arena::CreateMessage<rc::RobotId>(arena_.get());

    result->set_color(robotIdColorFromTeam(team));
    result->set_number(number);

    return result;
  }

  observer_ptr<rc::Point2Df> point2DfFromVector2(const tp::Vector2& vector2) {
    observer_ptr result = Arena::CreateMessage<rc::Point2Df>(arena_.get());

    result->set_x(vector2.x() * kFMetersToMillimeters);
    result->set_y(vector2.y() * kFMetersToMillimeters);

    return result;
  }

  observer_ptr<google::protobuf::Duration> durationFromSeconds(float seconds) {
    observer_ptr result = Arena::CreateMessage<google::protobuf::Duration>(arena_.get());

    auto whole_seconds = static_cast<int64_t>(seconds);
    auto nanoseconds = static_cast<int32_t>((seconds - static_cast<float>(whole_seconds))
                                            * kFSecondsToNanoseconds);

    result->set_seconds(whole_seconds);
    result->set_nanos(nanoseconds);

    return result;
  }

  observer_ptr<google::protobuf::Timestamp> timestampFromUnixNanos(uint64_t nanoseconds) {
    observer_ptr result = Arena::CreateMessage<google::protobuf::Timestamp>(arena_.get());

    uint64_t whole_seconds = nanoseconds / kULLSecondsToNanoseconds;
    auto nanos = static_cast<int32_t>(nanoseconds % kULLSecondsToNanoseconds);

    result->set_seconds(static_cast<int64_t>(whole_seconds));
    result->set_nanos(nanos);

    return result;
  }

  [[nodiscard]] rc::Team teamFromTeam(tp::Team by_team) const {
    switch (by_team) {
      case tp::Team::BLUE: {
        if (home_is_blue_team_) {
          return rc::Team::GameEvent_Team_TEAM_HOME;
        }
        return rc::Team::GameEvent_Team_TEAM_AWAY;
      }
      case tp::Team::YELLOW: {
        if (home_is_blue_team_) {
          return rc::Team::GameEvent_Team_TEAM_AWAY;
        }
        return rc::Team::GameEvent_Team_TEAM_HOME;
      }
      default: {
        return rc::Team::GameEvent_Team_TEAM_UNSPECIFIED;
      }
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

  observer_ptr<Arena> arena_;
  bool home_is_blue_team_ = false;
};

} // namespace

GameEventMapper::GameEventMapper(bool home_is_blue_team, observer_ptr<Arena> arena) :
    home_is_blue_team_(home_is_blue_team),
    arena_(arena) {}

observer_ptr<rc::GameEvent> GameEventMapper::fromTimestampAndGameControllerEvent(
    std::unique_ptr<google::protobuf::Timestamp> timestamp,
    const tp::GameEvent& game_event) {
  using enum tp::GameEvent::EventCase;

  MapperInternal mapper{home_is_blue_team_, arena_};

  observer_ptr result = Arena::CreateMessage<rc::GameEvent>(arena_.get());
  result->set_allocated_timestamp(timestamp.release());

  switch (game_event.event_case()) {
    case kBallLeftFieldTouchLine: {
      result->set_allocated_ball_left_field_touch_line(
          mapper.ballLeftFieldFromBallLeftField(game_event.ball_left_field_touch_line()).get());
      break;
    }
    case kBallLeftFieldGoalLine: {
      result->set_allocated_ball_left_field_goal_line(
          mapper.ballLeftFieldFromBallLeftField(game_event.ball_left_field_goal_line()).get());
      break;
    }
    case kBoundaryCrossing: {
      result->set_allocated_ball_left_field_boundary(
          mapper.ballLeftFieldBoundaryFromBoundaryCrossing(game_event.boundary_crossing()).get());
      break;
    }
    case kAimlessKick: {
      result->set_allocated_aimless_kick(
          mapper.aimlessKickFromAimlessKick(game_event.aimless_kick()).get());
      break;
    }
    case kKeeperHeldBall: {
      result->set_allocated_goalkeeper_held_ball(
          mapper.goalkeeperHeldBallFromKeeperHeldBall(game_event.keeper_held_ball()).get());
      break;
    }
    case kAttackerTooCloseToDefenseArea: {
      result->set_allocated_robot_too_close_to_defense_area(
          mapper
              .robotTooCloseToDefenseAreaFromAttackerTooCloseToDefenseArea(
                  game_event.attacker_too_close_to_defense_area())
              .get());
      break;
    }
    case kDefenderInDefenseArea: {
      result->set_allocated_robot_in_defense_area(
          mapper.robotInDefenseAreaFromDefenderInDefenseArea(game_event.defender_in_defense_area())
              .get());
      break;
    }
    case kBotPushedBot: {
      result->set_allocated_robot_pushed_robot(
          mapper.robotPushedRobotFromBotPushedBot(game_event.bot_pushed_bot()).get());
      break;
    }
    case kBotHeldBallDeliberately: {
      result->set_allocated_robot_held_ball_deliberately(
          mapper
              .robotHeldBallDeliberatelyFromBotHeldBallDeliberately(
                  game_event.bot_held_ball_deliberately())
              .get());
      break;
    }
    case kBotDribbledBallTooFar: {
      result->set_allocated_robot_dribbled_ball_too_far(
          mapper
              .robotDribbledBallTooFarFromBotDribbledBallTooFar(
                  game_event.bot_dribbled_ball_too_far())
              .get());
      break;
    }
    case kBotTippedOver: {
      result->set_allocated_robot_tipped_over(
          mapper.robotTippedOverFromBotTippedOver(game_event.bot_tipped_over()).get());
      break;
    }
    case kAttackerTouchedBallInDefenseArea: {
      result->set_allocated_robot_touched_ball_in_defense_area(
          mapper
              .robotTouchedBallInDefenseAreaFromAttackerTouchedBallInDefenseArea(
                  game_event.attacker_touched_ball_in_defense_area())
              .get());
      break;
    }
    case kBotKickedBallTooFast: {
      result->set_allocated_robot_kicked_ball_too_fast(
          mapper
              .robotKickedBallTooFastFromBotKickedBallTooFast(game_event.bot_kicked_ball_too_fast())
              .get());
      break;
    }
    case kBotCrashUnique: {
      result->set_allocated_robot_crash_unique(
          mapper.robotCrashUniqueFromBotCrashUnique(game_event.bot_crash_unique()).get());
      break;
    }
    case kBotCrashDrawn: {
      result->set_allocated_robot_crash_drawn(
          mapper.robotCrashDrawnFromBotCrashDrawn(game_event.bot_crash_drawn()).get());
      break;
    }
    case kBotTooFastInStop: {
      result->set_allocated_robot_too_fast_in_stop(
          mapper.robotTooFastInStopFromBotTooFastInStop(game_event.bot_too_fast_in_stop()).get());
      break;
    }
    case kDefenderTooCloseToKickPoint: {
      result->set_allocated_robot_too_close_to_kick_point(
          mapper
              .robotTooCloseToKickPointFromDefenderTooCloseToKickPoint(
                  game_event.defender_too_close_to_kick_point())
              .get());
      break;
    }
    case kBotInterferedPlacement: {
      result->set_allocated_robot_interfered_ball_placement(
          mapper
              .robotInterferedBallPlacementFromBotInterferedPlacement(
                  game_event.bot_interfered_placement())
              .get());
      break;
    }
    case kAttackerDoubleTouchedBall: {
      result->set_allocated_robot_double_touched_ball(
          mapper
              .robotDoubleTouchedBallFromAttackerDoubleTouchedBall(
                  game_event.attacker_double_touched_ball())
              .get());
      break;
    }
    case kNoProgressInGame: {
      result->set_allocated_no_progress_in_game(
          mapper.noProgressInGameFromNoProgressInGame(game_event.no_progress_in_game()).get());
      break;
    }
    case kMultipleCards: {
      result->set_allocated_multiple_cards(
          mapper.multipleCardsFromMultipleCards(game_event.multiple_cards()).get());
      break;
    }
    case kMultipleFouls: {
      result->set_allocated_multiple_fouls(
          mapper.multipleFoulsFromMultipleFouls(game_event.multiple_fouls()).get());
      break;
    }
    case kTooManyRobots: {
      result->set_allocated_too_many_robots(
          mapper.tooManyRobotsFromTooManyRobots(game_event.too_many_robots()).get());
      break;
    }
    case kPlacementSucceeded: {
      result->set_allocated_ball_placement_succeeded(
          mapper.ballPlacementSucceededFromPlacementSucceeded(game_event.placement_succeeded())
              .get());
      break;
    }
    case kPlacementFailed: {
      result->set_allocated_ball_placement_failed(
          mapper.ballPlacementFailedFromPlacementFailed(game_event.placement_failed()).get());
      break;
    }
    case kPenaltyKickFailed: {
      result->set_allocated_penalty_kick_failed(
          mapper.penaltyKickFailedFromPenaltyKickFailed(game_event.penalty_kick_failed()).get());
      break;
    }
    case kPossibleGoal: {
      result->set_allocated_possible_goal(mapper.goalFromGoal(game_event.possible_goal()).get());
      break;
    }
    case kGoal: {
      result->set_allocated_goal(mapper.goalFromGoal(game_event.goal()).get());
      break;
    }
    case kInvalidGoal: {
      result->set_allocated_invalid_goal(mapper.goalFromGoal(game_event.invalid_goal()).get());
      break;
    }
    case kBotSubstitution: {
      result->set_allocated_robot_substitution(
          mapper.robotSubstitutionFromBotSubstitution(game_event.bot_substitution()).get());
      break;
    }
    case kChallengeFlag: {
      result->set_allocated_challenge_flag(
          mapper.challengeFlagFromChallengeFlag(game_event.challenge_flag()).get());
      break;
    }
    case kEmergencyStop: {
      result->set_allocated_emergency_stop(
          mapper.emergencyStopFromEmergencyStop(game_event.emergency_stop()).get());
      break;
    }
    case kUnsportingBehaviorMinor: {
      result->set_allocated_unsporting_behavior_minor(
          mapper
              .unsportingBehaviorMinorFromUnsportingBehaviorMinor(
                  game_event.unsporting_behavior_minor())
              .get());
      break;
    }
    case kUnsportingBehaviorMajor: {
      result->set_allocated_unsporting_behavior_major(
          mapper
              .unsportingBehaviorMajorFromUnsportingBehaviorMajor(
                  game_event.unsporting_behavior_major())
              .get());
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

} // namespace referee

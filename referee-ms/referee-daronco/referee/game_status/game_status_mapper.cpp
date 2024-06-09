#include "referee/game_status/game_status_mapper.h"

#include <google/protobuf/arena.h>
#include <google/protobuf/util/time_util.h>
#include <protocols/common/game_command.pb.h>
#include <protocols/common/match_type.pb.h>
#include <protocols/common/team.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>
#include <utility>

namespace referee {
namespace {

using ::google::protobuf::Arena;
using ::google::protobuf::util::TimeUtil;
using ::robocin::object_ptr;

namespace rc {

using ::protocols::common::MatchType;
using ::protocols::common::Team;
using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::MatchType;
using ::protocols::third_party::game_controller::Referee;

} // namespace tp

object_ptr<google::protobuf::Duration> durationFromMicros(int64_t microseconds,
                                                          object_ptr<Arena> arena) {
  return Arena::Create<google::protobuf::Duration>(arena.get(),
                                                   TimeUtil::MicrosecondsToDuration(microseconds));
}

object_ptr<google::protobuf::Timestamp> timestampFromUnixMicros(uint64_t microseconds,
                                                                object_ptr<Arena> arena) {
  return Arena::Create<google::protobuf::Timestamp>(
      arena.get(),
      TimeUtil::MicrosecondsToTimestamp(static_cast<int64_t>(microseconds)));
}

rc::MatchType matchTypeFromMatchType(tp::MatchType match_type) {
  using enum rc::MatchType;
  using enum tp::MatchType;

  switch (match_type) {
    case GROUP_PHASE: return MATCH_TYPE_GROUP_PHASE;
    case ELIMINATION_PHASE: return MATCH_TYPE_ELIMINATION_PHASE;
    case FRIENDLY: return MATCH_TYPE_FRIENDLY;
    default: return MATCH_TYPE_UNSPECIFIED;
  }
  std::unreachable();
}

} // namespace

GameStatusMapper::GameStatusMapper(object_ptr<ITeamStatusMapper> team_status_mapper,
                                   object_ptr<IGameStageMapper> game_stage_mapper,
                                   object_ptr<IGameCommandMapper> game_command_mapper,
                                   object_ptr<IGameEventMapper> game_event_mapper,
                                   object_ptr<Arena> arena) :
    team_status_mapper_(team_status_mapper),
    game_stage_mapper_(game_stage_mapper),
    game_command_mapper_(game_command_mapper),
    game_event_mapper_(game_event_mapper),
    arena_(arena) {}

object_ptr<rc::GameStatus>
GameStatusMapper::gameCommandFromDetectionAndReferee(const rc::Detection& detection,
                                                     const tp::Referee& referee) {
  object_ptr result = Arena::Create<rc::GameStatus>(arena_.get());
  bool home_is_blue_team = !referee.blue_team_on_positive_half();

  result->set_source_id(referee.source_identifier());
  result->set_description(referee.status_message());
  result->unsafe_arena_set_allocated_timestamp(
      timestampFromUnixMicros(static_cast<int64_t>(referee.packet_timestamp()), arena_).get());
  result->set_match_type(matchTypeFromMatchType(referee.match_type()));

  result->unsafe_arena_set_allocated_home_team(
      team_status_mapper_->fromTeamAndRefereeTeamInfo(rc::Team::TEAM_HOME, referee).get());
  result->unsafe_arena_set_allocated_away_team(
      team_status_mapper_->fromTeamAndRefereeTeamInfo(rc::Team::TEAM_AWAY, referee).get());

  result->set_game_stage(game_stage_mapper_->gameStageFromRefereeStage(referee.stage()));
  result->unsafe_arena_set_allocated_game_stage_time_left(
      durationFromMicros(referee.stage_time_left(), arena_).get());

  result->set_total_commands_issued(referee.command_counter());
  result->unsafe_arena_set_allocated_command_issued_timestamp(
      timestampFromUnixMicros(static_cast<int64_t>(referee.command_timestamp()), arena_).get());
  result->unsafe_arena_set_allocated_command(
      game_command_mapper_->fromDetectionAndReferee(detection, referee, /*is_next_command=*/false)
          .get());
  result->unsafe_arena_set_allocated_next_command(
      game_command_mapper_->fromDetectionAndReferee(detection, referee, /*is_next_command=*/true)
          .get());

  *result->mutable_game_events() = *game_event_mapper_->gameEventsFromReferee(referee);
  *result->mutable_game_events_proposals()
      = *game_event_mapper_->gameEventsProposalFromReferee(referee);

  return result;
}

} // namespace referee

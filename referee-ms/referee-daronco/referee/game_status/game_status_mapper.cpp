#include "referee/game_status/game_status_mapper.h"

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

google::protobuf::Duration durationFromMicros(int64_t microseconds) {
  return TimeUtil::MicrosecondsToDuration(microseconds);
}

google::protobuf::Timestamp timestampFromUnixMicros(uint64_t microseconds) {
  return TimeUtil::MicrosecondsToTimestamp(static_cast<int64_t>(microseconds));
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
                                   object_ptr<IGameEventsMapper> game_events_mapper) :
    team_status_mapper_{team_status_mapper},
    game_stage_mapper_{game_stage_mapper},
    game_command_mapper_{game_command_mapper},
    game_events_mapper_{game_events_mapper} {}

rc::GameStatus GameStatusMapper::fromDetectionAndReferee(const rc::Detection& detection,
                                                         const tp::Referee& referee) {
  rc::GameStatus result;
  bool home_is_blue_team = !referee.blue_team_on_positive_half();

  result.set_source_id(referee.source_identifier());
  result.set_description(referee.status_message());

  *result.mutable_timestamp()
      = timestampFromUnixMicros(static_cast<int64_t>(referee.packet_timestamp()));

  *result.mutable_timestamp()
      = timestampFromUnixMicros(static_cast<int64_t>(referee.packet_timestamp()));
  result.set_match_type(matchTypeFromMatchType(referee.match_type()));

  *result.mutable_home_team()
      = team_status_mapper_->fromTeamAndRefereeTeamInfo(rc::Team::TEAM_HOME, referee);
  *result.mutable_away_team()
      = team_status_mapper_->fromTeamAndRefereeTeamInfo(rc::Team::TEAM_AWAY, referee);

  result.set_game_stage(game_stage_mapper_->fromRefereeStage(referee.stage()));
  *result.mutable_game_stage_time_left() = durationFromMicros(referee.stage_time_left());

  result.set_total_commands_issued(referee.command_counter());
  *result.mutable_command_issued_timestamp()
      = timestampFromUnixMicros(static_cast<int64_t>(referee.command_timestamp()));
  *result.mutable_command()
      = game_command_mapper_->fromDetectionAndReferee(detection,
                                                      referee,
                                                      /*is_next_command=*/false);
  *result.mutable_next_command()
      = game_command_mapper_->fromDetectionAndReferee(detection,
                                                      referee,
                                                      /*is_next_command=*/true);

  *result.mutable_game_events() = game_events_mapper_->fromReferee(referee);
  *result.mutable_game_events_proposals() = game_events_mapper_->proposalsFromReferee(referee);

  return result;
}

} // namespace referee

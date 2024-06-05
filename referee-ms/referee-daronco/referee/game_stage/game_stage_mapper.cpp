#include "referee/game_stage/game_stage_mapper.h"

namespace referee {
namespace {

namespace rc {

using ::protocols::common::GameStage;

} // namespace rc

namespace tp {

using RefereeStage = ::protocols::third_party::game_controller::Referee::Stage;

} // namespace tp

} // namespace

rc::GameStage GameStageMapper::gameStageFromRefereeStage(tp::RefereeStage stage) {
  using enum tp::RefereeStage;
  using enum rc::GameStage;

  switch (stage) {
    case Referee_Stage_NORMAL_FIRST_HALF_PRE: return GAME_STAGE_NORMAL_FIRST_HALF_PRE;
    case Referee_Stage_NORMAL_FIRST_HALF: return GAME_STAGE_NORMAL_FIRST_HALF;
    case Referee_Stage_NORMAL_HALF_TIME: return GAME_STAGE_NORMAL_HALF_TIME;
    case Referee_Stage_NORMAL_SECOND_HALF_PRE: return GAME_STAGE_NORMAL_SECOND_HALF_PRE;
    case Referee_Stage_NORMAL_SECOND_HALF: return GAME_STAGE_NORMAL_SECOND_HALF;
    case Referee_Stage_EXTRA_TIME_BREAK: return GAME_STAGE_EXTRA_TIME_BREAK;
    case Referee_Stage_EXTRA_FIRST_HALF_PRE: return GAME_STAGE_EXTRA_FIRST_HALF_PRE;
    case Referee_Stage_EXTRA_FIRST_HALF: return GAME_STAGE_EXTRA_FIRST_HALF;
    case Referee_Stage_EXTRA_HALF_TIME: return GAME_STAGE_EXTRA_HALF_TIME;
    case Referee_Stage_EXTRA_SECOND_HALF_PRE: return GAME_STAGE_EXTRA_SECOND_HALF_PRE;
    case Referee_Stage_EXTRA_SECOND_HALF: return GAME_STAGE_EXTRA_SECOND_HALF;
    case Referee_Stage_PENALTY_SHOOTOUT_BREAK: return GAME_STAGE_PENALTY_SHOOTOUT_BREAK;
    case Referee_Stage_PENALTY_SHOOTOUT: return GAME_STAGE_PENALTY_SHOOTOUT;
    case Referee_Stage_POST_GAME: return GAME_STAGE_POST_GAME;
  }

  return GAME_STAGE_UNSPECIFIED;
}

} // namespace referee

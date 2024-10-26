#include <navigation/processing/utils/move_task_state.h>

namespace navigation {

bool MoveTaskState::refereeRequestFinish(const rc::GameCommand& command) {
  return command.has_halt();
}

bool MoveTaskState::checkFinishState(const rc::Point2Df& allyPosition,
                                     const rc::GoToPoint& goToPointTask,
                                     const rc::Point2Df& lastPosition) {

  rc::Point2Df target = {goToPointTask.target().x(), goToPointTask.target().y()};
  bool robotCloseToDestinationInTask
      = target.distanceTo(allyPosition) < TOLERANCE_TO_CONSIDER_SAME_TARGET;
  bool finishPathToTask = lastPosition.distanceTo(allyPosition) < TOLERANCE_TO_CONSIDER_SAME_TARGET;

  return robotCloseToDestinationInTask || finishPathToTask;
}

[[nodiscard]] bool MoveTaskState::checkChangeTarget(const rc::GoToPoint& goToPointTask) {

  rc::Point2Df target = {goToPointTask.target().x(), goToPointTask.target().y()};
  return currentDesiredTarget_.distanceTo(target) > TOLERANCE_TO_CONSIDER_SAME_TARGET;
}

void MoveTaskState::updateState(const rc::Robot& ally,
                                const rc::GameCommand& command,
                                const rc::GoToPoint& goToPointTask,
                                const rc::PathNode& lastNode) {

  rc::Point2Df last_node_pos = {lastNode.position().x(), lastNode.position().y()};
  rc::Point2Df lastPositionInPath = last_node_pos * M_to_MM;

  if (!refereeRequestFinish(command)) {
    if (state_ != SkillMoveState::Unknown
        && checkFinishState(rc::Point2Df(ally.position().x(), ally.position().y()),
                            goToPointTask,
                            lastPositionInPath)) {
      state_ = SkillMoveState::Finished;
      currentDesiredTarget_ = {0, 0};
    } else {
      if (currentDesiredTarget_.x == 0 and currentDesiredTarget_.y == 0) {

        state_ = SkillMoveState::Started;
        currentTarget_ = last_node_pos;
        estimateTimeToTarget_ = lastNode.time();
        runningToTarget_.restart();
        currentDesiredTarget_ = {goToPointTask.target().x(), goToPointTask.target().y()};
        globalRunningToTarget_.restart();

      } else if (checkChangeTarget(goToPointTask)) {

        state_ = SkillMoveState::ChangedTarget;
        currentDesiredTarget_ = {goToPointTask.target().x(), goToPointTask.target().y()};

      } else {
        state_ = SkillMoveState::Running;
      }
    }
  } else {
    state_ = SkillMoveState::Unknown;
  }
}

[[nodiscard]] SkillMoveState MoveTaskState::currentState() { return state_; }

void MoveTaskState::initialSetupState(const rc::PathNode& lastNode) {
  currentTarget_ = {lastNode.position().x(), lastNode.position().y()};
  estimateTimeToTarget_ = lastNode.time();
  runningToTarget_.restart();
};

void MoveTaskState::resetRunningToTargetTimer() {
  if (runningToTarget_.isValid()) {
    runningToTarget_.invalidate();
  }
}

double MoveTaskState::executionTimeError(const rc::PathNode& lastNode) {
  return estimateTimeToTarget_
         - (static_cast<double>(runningToTarget_.elapsed().frames()) / 1e9 + lastNode.time());
}
} // namespace navigation

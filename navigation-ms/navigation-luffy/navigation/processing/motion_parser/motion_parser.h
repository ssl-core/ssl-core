#ifndef NAVIGATION_PROCESSING_MOTION_PARSER_H
#define NAVIGATION_PROCESSING_MOTION_PARSER_H

#include <navigation/processing/utils/robot_move.h>
#include <protocols/behavior/behavior_unification.pb.h>
#include <protocols/behavior/motion.pb.h>
#include <protocols/perception/detection.pb.h>

#define PI 3.14159265358979323846

namespace navigation {

class IMotionParser {
 public:
  IMotionParser() = default;

  IMotionParser(const IMotionParser& other) = delete;
  IMotionParser& operator=(const IMotionParser& other) = delete;
  IMotionParser(IMotionParser&& other) = default;
  IMotionParser& operator=(IMotionParser&& other) = default;

  virtual ~IMotionParser() = default;

  virtual RobotMove fromGoToPoint(const ::protocols::behavior::GoToPoint& go_to_point,
                                  const ::protocols::perception::Robot& robot)
      = 0;

  virtual RobotMove fromRotateInPoint(const ::protocols::behavior::RotateInPoint& rotate_in_point,
                                      const ::protocols::perception::Robot& robot)
      = 0;

  virtual RobotMove fromRotateOnSelf(const ::protocols::behavior::RotateOnSelf& rotate_on_self,
                                     const ::protocols::perception::Robot& robot)
      = 0;
};

class MotionParser : public IMotionParser {
 public:
  MotionParser();

  RobotMove fromGoToPoint(const ::protocols::behavior::GoToPoint& go_to_point,
                          const ::protocols::perception::Robot& robot) override;

  RobotMove fromRotateInPoint(const ::protocols::behavior::RotateInPoint& rotate_in_point,
                              const ::protocols::perception::Robot& robot) override;

  RobotMove fromRotateOnSelf(const ::protocols::behavior::RotateOnSelf& rotate_on_self,
                             const ::protocols::perception::Robot& robot) override;
};

} // namespace navigation

#endif // NAVIGATION_PROCESSING_MOTION_PARSER_H

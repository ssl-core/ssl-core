// Frame.h
#ifndef VISION_SRC_FRAME_H
#define VISION_SRC_FRAME_H

#include "vision/src/ball/ball.h"
#include "vision/src/field/field.h"
#include "vision/src/robot/robot.h"

#include <iostream>
#include <vector>

class Frame {
 public:
  Frame(int id,
        const std::vector<Ball>& balls,
        const std::vector<Robot>& robots,
        const Field& field);

  friend std::ostream& operator<<(std::ostream& os, const Frame& frame);

  // - MARK: Getters.
  [[nodiscard]] int getId() const;                    // Getter for id
  [[nodiscard]] std::vector<Ball> getBalls() const;   // Getter for balls
  [[nodiscard]] std::vector<Robot> getRobots() const; // Getter for robots
  [[nodiscard]] Field getField() const;               // Getter for field

  // - MARK: JSON Support.
  [[nodiscard]] std::string toJson() const;
  [[nodiscard]] static Frame fromJson([[maybe_unused]] const std::string& json);

 private:
  int id_;
  std::vector<Ball> balls_;
  std::vector<Robot> robots_;
  Field field_;
};

#endif // VISION_SRC_FRAME_H

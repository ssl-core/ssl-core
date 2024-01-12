// Frame.h
#ifndef FRAME_H
#define FRAME_H

#include "../Ball/Ball.h"
#include "../Field/Field.h"
#include "../Robot/Robot.h"

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

#endif // FRAME_H

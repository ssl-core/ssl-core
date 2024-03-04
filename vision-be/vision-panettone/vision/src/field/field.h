// Field.h
#ifndef VISION_SRC_FIELD_H
#define VISION_SRC_FIELD_H

#include <ostream>

class Field {
 public:
  Field(double length,
        double width,
        double goal_depth,
        double goal_width,
        double penalty_area_depth,
        double penalty_area_width,
        double boundary_width,
        double goal_center_to_penalty_mark);

  friend std::ostream& operator<<(std::ostream& os, const Field& field);

  [[nodiscard]] inline double getLength() const { return length_; };
  [[nodiscard]] inline double getWidth() const { return width_; };
  [[nodiscard]] inline double getGoalDepth() const { return goal_depth_; };
  [[nodiscard]] inline double getGoalWidth() const { return goal_width_; };
  [[nodiscard]] inline double getPenaltyAreaDepth() const { return penalty_area_depth_; };
  [[nodiscard]] inline double getPenaltyAreaWidth() const { return penalty_area_width_; };
  [[nodiscard]] inline double getBoundaryWidth() const { return boundary_width_; };
  [[nodiscard]] inline double getGoalCenterToPenaltyMark() const { return goal_center_to_penalty_mark_; };
  [[nodiscard]] inline double getHeight() const { return width_; };

  

 private:
  double length_;
  double width_;
  double goal_depth_;
  double goal_width_;
  double penalty_area_depth_;
  double penalty_area_width_;
  double boundary_width_;
  double goal_center_to_penalty_mark_;
};

// Static constant instance definition outside the class
static const Field kDefaultField = Field(/*length=*/9.0,
                                         /*width=*/6.0,
                                         /*goal_depth=*/0.6,
                                         /*goal_width=*/1.0,
                                         /*penalty_area_depth=*/1.8,
                                         /*penalty_area_width=*/3.0,
                                         /*boundary_width=*/0.3,
                                         /*goal_center_to_penalty_mark=*/1.0);

#endif // VISION_SRC_FIELD_H

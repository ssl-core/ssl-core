// Field.h
#ifndef FIELD_H
#define FIELD_H

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

#endif // FIELD_H

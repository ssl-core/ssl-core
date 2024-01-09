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

#endif // FIELD_H

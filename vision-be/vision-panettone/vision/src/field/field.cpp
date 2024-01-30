// Field.cpp
#include "vision/src/field/field.h"

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
Field::Field(double length,
             double width,
             double goal_depth,
             double goal_width,
             double penalty_area_depth,
             double penalty_area_width,
             double boundary_width,
             double goal_center_to_penalty_mark) :
    length_(length),
    width_(width),
    goal_depth_(goal_depth),
    goal_width_(goal_width),
    penalty_area_depth_(penalty_area_depth),
    penalty_area_width_(penalty_area_width),
    boundary_width_(boundary_width),
    goal_center_to_penalty_mark_(goal_center_to_penalty_mark) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

std::ostream& operator<<(std::ostream& os, const Field& field) {
  os << "Field(length=" << field.length_ << ", width=" << field.width_
     << ", goal_depth=" << field.goal_depth_ << ", goal_width=" << field.goal_width_
     << ", penalty_area_depth=" << field.penalty_area_depth_
     << ", penalty_area_width=" << field.penalty_area_width_
     << ", boundary_width=" << field.boundary_width_
     << ", goal_center_to_penalty_mark=" << field.goal_center_to_penalty_mark_ << ")";
  return os;
}

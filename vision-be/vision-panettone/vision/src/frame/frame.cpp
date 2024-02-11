// Frame.cpp
#include "vision/src/frame/frame.h"

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
Frame::Frame(int id,
             const std::vector<Ball>& balls,
             const std::vector<Robot>& robots,
             const Field& field) :
    id_(id),
    balls_(balls),
    robots_(robots),
    field_(field) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

std::ostream& operator<<(std::ostream& os, const Frame& frame) {
  os << "Frame(id=" << frame.id_ << ", balls=[";
  for (const auto& ball : frame.balls_) {
    os << ball << ", ";
  }
  os << "], robots=[";
  for (const auto& robot : frame.robots_) {
    os << robot << ", ";
  }
  os << "], field=";
  os << frame.field_;
  os << ")";
  return os;
}

Frame Frame::fromJson([[maybe_unused]] const std::string& json) {
  // TODO(protobuf): Use Protobuf to parse.
  return Frame{/*id=*/0, {}, {}, kDefaultField};
}

int Frame::getId() const {
  return id_;
}
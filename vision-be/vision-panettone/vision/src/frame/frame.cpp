// Frame.cpp
#include "vision/src/frame/frame.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

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

int Frame::getId() const { return id_; }

std::vector<Ball> Frame::getBalls() const { return balls_; }

std::vector<Robot> Frame::getRobots() const { return robots_; }

Field Frame::getField() const { return field_; }

bsoncxx::builder::stream::document Frame::getMongoDocument() const {
  // Create your ID document
  bsoncxx::builder::stream::document document{};

  // add id to the document
  document << "_id" << id_;

  // add robots array
  bsoncxx::builder::stream::array robots_array{};
  for (const auto& robot : robots_) {
    bsoncxx::builder::stream::document robot_doc{};
    robot_doc << "uuid" << robot.getUuid();
    robot_doc << "id" << robot.getId();
    robot_doc << "color" << static_cast<int>(robot.getColor());
    robot_doc << "position" << bsoncxx::builder::stream::open_array << robot.getPosition().getX()
              << robot.getPosition().getY() << bsoncxx::builder::stream::close_array;
    robot_doc << "angle" << robot.getAngle();
    robot_doc << "velocity" << bsoncxx::builder::stream::open_array << robot.getVelocity().getX()
              << robot.getVelocity().getY() << bsoncxx::builder::stream::close_array;
    robot_doc << "angular_velocity" << robot.getAngularVelocity();
    robot_doc << "source" << static_cast<int>(robot.getSource());
    robots_array << robot_doc;
  }
  document << "robots" << robots_array;

  // add balls array
  bsoncxx::builder::stream::array balls_array{};
  for (const auto& ball : balls_) {
    bsoncxx::builder::stream::document ball_doc{};
    ball_doc << "uuid" << ball.getUuid();
    ball_doc << "source" << static_cast<int>(ball.getSource());
    ball_doc << "position" << bsoncxx::builder::stream::open_array << ball.getPosition().getX()
             << ball.getPosition().getY() << bsoncxx::builder::stream::close_array;
    ball_doc << "velocity" << bsoncxx::builder::stream::open_array << ball.getVelocity().getX()
             << ball.getVelocity().getY() << bsoncxx::builder::stream::close_array;
    balls_array << ball_doc;
  }
  document << "balls" << balls_array;

  // add field document
  bsoncxx::builder::stream::document field_doc{};

  field_doc << "length" << field_.getLength();
  field_doc << "width" << field_.getWidth();
  field_doc << "goal_width" << field_.getGoalWidth();
  field_doc << "goal_depth" << field_.getGoalDepth();
  field_doc << "penalty_area_width" << field_.getPenaltyAreaWidth();
  field_doc << "penalty_area_depth" << field_.getPenaltyAreaDepth();
  field_doc << "boundary_width" << field_.getBoundaryWidth();
  field_doc << "goal_center_to_penalty_mark" << field_.getGoalCenterToPenaltyMark();
  document << "field" << field_doc;

  return document;
}

#include "qt_monolithic/utility.h"

#include <protocols/common/robot_id.pb.h>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/ui/messages.pb.h>
#include <protocols/vision/frame.pb.h>
#include <google/protobuf/util/time_util.h>

namespace vision::qt_monolithic {
namespace {

using protocols::third_party::detection::SSL_DetectionRobot;
using protocols::third_party::detection::SSL_WrapperPacket;
using protocols::vision::Frame;
using protocols::vision::Robot;

using RobotIdColor = protocols::common::RobotId::Color;
using robocin::ZmqDatagram;

google::protobuf::Timestamp GetTimestampNow() {
  return google::protobuf::util::TimeUtil::GetCurrentTime();
}

Robot WrapperRobotToRobot(const SSL_DetectionRobot& packet_robot, RobotIdColor color) {
  Robot robot;

  auto& robot_id = *robot.mutable_robot_id();
  robot_id.set_number(static_cast<int>(packet_robot.robot_id()));
  robot_id.set_color(color);

  robot.set_confidence(packet_robot.confidence());

  auto& position = *robot.mutable_position();
  position.set_x(packet_robot.x());
  position.set_y(packet_robot.y());

  robot.set_angle(packet_robot.orientation());

  // just to increase message size.
  auto& velocity = *robot.mutable_velocity();
  velocity.set_x(0);
  velocity.set_y(0);

  robot.set_angular_velocity(0);

  return robot;
}

Frame WrapperPacketToFrame(int serial_id, int field_serial_id, const SSL_WrapperPacket& packet) {
  Frame frame;

  auto& properties = *frame.mutable_properties();
  properties.set_serial_id(serial_id++);
  *properties.mutable_created_at() = GetTimestampNow();
  properties.set_fps(60.0F); // NOLINT(*numbers*)

  for (const auto& packet_ball : packet.detection().balls()) {
    auto& ball = *frame.add_balls();

    ball.set_confidence(packet_ball.confidence());

    auto& position = *ball.mutable_position();
    position.set_x(packet_ball.x());
    position.set_y(packet_ball.y());
    position.set_z(packet_ball.z());

    // just to increase message size.
    auto& velocity = *ball.mutable_velocity();
    velocity.set_x(0);
    velocity.set_y(0);
    velocity.set_z(0);
  }

  for (const auto& packet_robot : packet.detection().robots_yellow()) {
    *frame.add_robots()
        = WrapperRobotToRobot(packet_robot, RobotIdColor::RobotId_Color_COLOR_YELLOW);
  }
  for (const auto& packet_robot : packet.detection().robots_blue()) {
    *frame.add_robots()
        = WrapperRobotToRobot(packet_robot, RobotIdColor::RobotId_Color_COLOR_BLUE);
  }

  if (packet.has_geometry()) {
    auto& field = *frame.mutable_field();
    field.set_serial_id(field_serial_id++);

    // NOLINTBEGIN(*conversions*)
    field.set_length(packet.geometry().field().field_length());
    field.set_width(packet.geometry().field().field_width());
    field.set_goal_depth(packet.geometry().field().goal_depth());
    field.set_goal_width(packet.geometry().field().goal_width());
    field.set_penalty_area_depth(packet.geometry().field().penalty_area_depth());
    field.set_penalty_area_width(packet.geometry().field().penalty_area_width());
    field.set_boundary_width(packet.geometry().field().boundary_width());
    field.set_goal_center_to_penalty_mark(packet.geometry().field().goal_center_to_penalty_mark());
    // NOLINTEND(*conversions*)
  }

  return frame;
}

Frame ParseMessage(std::string_view message, int id, int serial_id, int field_serial_id) {
  if (id == 0) {
    SSL_WrapperPacket packet;
    packet.ParseFromString(std::string(message));

    Frame frame = WrapperPacketToFrame(serial_id, field_serial_id, packet);
    return frame;
  }

  Frame frame;
  frame.ParseFromString(std::string(message));
  return frame;
}

} // namespace

AModule::AModule(int id, float sleep_duration_ms)
  : id_(id),
    sleep_duration_ms_(sleep_duration_ms) {
}

void AModule::Run() {
  thread_ = std::make_unique<std::jthread>([this]() {
    ParallelRun();
  });
}

void AModule::ParallelRun() {
  while (true) {
    std::vector<ZmqDatagram> packages;
    {
      std::unique_lock lock(mutex_);
      cv_.wait(lock, [&] { return !shared_packages_.empty(); });

      packages.swap(shared_packages_);
    }

    for (const auto& [_, message] : packages) {
      MockedSleep();

      ISendMessage({.topic = std::string(kTopic), .message = MakeNextMessage(message)});
    }
  }
}

void AModule::ISendMessage(const ZmqDatagram& message) {
  std::cout << std::format("Here {}...", id_) << std::endl;
  emit QtSendMessage(message);
}

std::string AModule::MakeNextMessage(std::string_view message) {
  return ParseMessage(message, id_, serial_id_, field_serial_id_).SerializeAsString();
}

void AModule::MockedSleep() {
  std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_duration_ms_));
}

void AModule::Connect(AModule& prev) {
  connect(&prev,
          &AModule::QtSendMessage,
          this,
          &AModule::ReceiveMessage,
          Qt::DirectConnection);
}

void AModule::ReceiveMessage(const ZmqDatagram& message) {
  {
    std::lock_guard lock(mutex_);
    if (message.topic.empty()) {
      return;
    }
    shared_packages_.push_back(message);
  }

  std::cout << std::format("[{}] Received at Module {}.", GetDateTimeNow(), id_) << std::endl;

  cv_.notify_one();
}

} // namespace vision::qt

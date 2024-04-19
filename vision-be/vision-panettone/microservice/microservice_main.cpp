#include <cassert>
#include <condition_variable>
#include <format>
#include <google/protobuf/util/time_util.h>
#include <mutex>
#include <protocols/common/robot_id.pb.h>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/vision/frame.pb.h>
#include <robocin/network/zmq_publisher_socket.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <span>
#include <string>
#include <thread>
#include <vector>

using protocols::third_party::detection::SSL_DetectionRobot;
using protocols::third_party::detection::SSL_WrapperPacket;
using protocols::vision::Frame;
using protocols::vision::Robot;

using RobotIdColor = protocols::common::RobotId::Color;

using robocin::ZmqDatagram;
using robocin::ZmqPublisherSocket;
using robocin::ZmqSubscriberSocket;

static constexpr std::string_view kTopic = "robocin";

template <class Tp>
[[gnu::always_inline]] inline void DoNotOptimize(Tp& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

template <class Tp>
[[gnu::always_inline]] inline void DoNotOptimize(Tp&& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

int reps_to_wait = 0;

std::unique_ptr<ZmqSubscriberSocket> sub;
std::unique_ptr<ZmqPublisherSocket> pub;

std::mutex mutex;
std::condition_variable cv;
std::vector<ZmqDatagram> packages;

void subscriberRun() {
  while (true) {
    {
      std::lock_guard lock(mutex);

      do {
        while (true) {
          auto message = sub->receive();
          if (message.topic.empty()) {
            break;
          }
          packages.push_back(message);
        }
      } while (packages.empty());
    }

    cv.notify_one();
  }
}

ZmqSubscriberSocket makeSubscriberSocket(int id);
ZmqPublisherSocket makePublisherSocket(int id);
Frame mapWrapperPacketToFrame(const SSL_WrapperPacket& packet);
void mockedSleep();

std::string parseMessage(std::string message, int id) {
  if (id == 0) {
    SSL_WrapperPacket packet;
    packet.ParseFromString(message);

    Frame frame = mapWrapperPacketToFrame(packet);
    return frame.SerializeAsString();
  }
  return message;
}

void publisherRun(int id) {
  while (true) {
    std::vector<ZmqDatagram> local_packages;
    {
      std::unique_lock lock(mutex);
      cv.wait(lock, [&] { return !packages.empty(); });

      local_packages.swap(packages);
    }

    for (const auto& [_, message] : local_packages) {
      mockedSleep();

      auto message_parsed = parseMessage(message, id);
      pub->send(kTopic, message_parsed);

      // if (id == 5) {
      //   std::cout << std::format("Sending at {} at {}!", id, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) << std::endl;
      // }
    }
  }
}

int main(int argc, char* argv[]) {
  assert(argc > 2);
  // argv[0] is relative binary name.

  std::span args{argv + 1, argv + argc - 1};
  // args[0] is the service_id.
  // args[1] is the number of loops cycles to wait.

  int service_id = std::stoi(args[0]);
  reps_to_wait = std::stoi(args[1]);

  std::cout << std::format("Service {} is running and waiting {} reps.!", service_id, reps_to_wait) << std::endl;

  sub = std::make_unique<ZmqSubscriberSocket>(makeSubscriberSocket(service_id));
  pub = std::make_unique<ZmqPublisherSocket>(makePublisherSocket(service_id));

  std::jthread subscriber_thread(subscriberRun);
  std::jthread publisher_thread(publisherRun, service_id);

  return 0;
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

ZmqSubscriberSocket makeSubscriberSocket(int id) {
  ZmqSubscriberSocket sub;

  if (id == 0) {
    static constexpr std::string_view k3rdPartyAddress = "ipc:///tmp/gateway-pub-th-parties.ipc";
    static constexpr std::string_view k3rdPartyTopic = "vision-third-party";

    sub.connect(k3rdPartyAddress, std::span{&k3rdPartyTopic, 1});
    std::cout << std::format("Service {} receiving from third party.", id) << std::endl;
  } else {
    std::string address = std::format("ipc:///tmp/channel{}.ipc", id - 1);
    std::cout << std::format("Service {} receiving from Service {}.", id, id - 1) << std::endl;
    sub.connect(address, std::span{&kTopic, 1});
  }

  return sub;
}

ZmqPublisherSocket makePublisherSocket(int id) {
  ZmqPublisherSocket pub;

  std::string address = std::format("ipc:///tmp/channel{}.ipc", id);
  std::cout << std::format("Service {} bind at: '{}'", id, address) << std::endl;
  pub.bind(address);

  return pub;
}

void mockedSleep() {
  for (int i = 0; i < reps_to_wait; ++i) {
    DoNotOptimize(i);
  }
}

google::protobuf::Timestamp protobufTimestampNow() {
  return google::protobuf::util::TimeUtil::GetCurrentTime();
}

Robot mapWrapperRobotToRobot(const SSL_DetectionRobot& packet_robot, RobotIdColor color) {
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
};

Frame mapWrapperPacketToFrame(const SSL_WrapperPacket& packet) {
  static uint64_t serial_id = 0;
  static uint64_t field_serial_id = 0;

  Frame frame;

  auto& properties = *frame.mutable_properties();
  properties.set_serial_id(serial_id++);
  *properties.mutable_created_at() = protobufTimestampNow();
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
        = mapWrapperRobotToRobot(packet_robot, RobotIdColor::RobotId_Color_COLOR_YELLOW);
  }
  for (const auto& packet_robot : packet.detection().robots_blue()) {
    *frame.add_robots()
        = mapWrapperRobotToRobot(packet_robot, RobotIdColor::RobotId_Color_COLOR_BLUE);
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

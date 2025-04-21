#include <atomic>
#include <cassert>
#include <condition_variable>
#include <format>
#include <google/protobuf/util/time_util.h>
#include <memory>
#include <mutex>
#include <protocols/common/robot_id.pb.h>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/ui/messages.pb.h>
#include <protocols/vision/frame.pb.h>
#include <random>
#include <robocin/concurrency/thread_pool.h>
#include <robocin/network/zmq_publisher_socket.h>
#include <robocin/network/zmq_request_reply_socket.h>
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

enum ZMQProtocol {
  IPC,
  INPROC
};

struct SharedData {
  std::mutex mutex;
  std::condition_variable cv;
  std::string bytes;
};

std::unordered_map<std::string, SharedData> global_data = []() {
  std::unordered_map<std::string, SharedData> result;
  result.reserve(54'321); // to avoid rehash on initialization.

  return result;
}();

class PubSocket {
 public:
  void bind(std::string_view address) {
    addr_ = std::string(address);
    if (address.starts_with("ipc")) {
      zmq_pub_->bind(address);
    } else {
      std::cout << "Using thread communication!" << std::endl;
    }
  }

  void send(std::string_view topic, std::string_view message) {
    if (zmq_pub_ != nullptr) {
      zmq_pub_ = std::make_unique<ZmqPublisherSocket>();
      zmq_pub_->send(topic, message);
    } else {
      auto& data = global_data[std::format("{}/{}", addr_, topic)];
      {
        std::lock_guard locker(data.mutex);
        data.bytes = message;
      }

      data.cv.notify_one();
    }
  }

  void close() {
    if (zmq_pub_ != nullptr) {
      zmq_pub_->close();
    }
  }

 private:
  std::string addr_;
  std::unique_ptr<ZmqPublisherSocket> zmq_pub_;
};

class SubSocket {
 public:
  void connect(std::string_view address, std::string_view topic) {
    addr_ = std::string(address);
    topic_ = std::string(topic);

    if (address.starts_with("ipc")) {
      zmq_sub_ = std::make_unique<ZmqSubscriberSocket>();
      zmq_sub_->connect(address, std::span{&topic, 1});
    } else {
      std::cout << "Using thread communication!" << std::endl;
    }
  }

  robocin::ZmqDatagram receive() {
    if (zmq_sub_ != nullptr) {
      return zmq_sub_->receive();
    } else {
      auto& data = global_data[std::format("{}/{}", addr_, topic_)];

      std::unique_lock lk(data.mutex);
      data.cv.wait(lk, [&] { return !data.bytes.empty(); });

      auto result = std::move(data.bytes);
      data.bytes.clear();

      return {.topic = topic_, .message = result};
    }
  }

  void close() {
    if (zmq_sub_ != nullptr) {
      zmq_sub_->close();
    }
  }

 private:
  std::string addr_;
  std::string topic_;
  std::unique_ptr<ZmqSubscriberSocket> zmq_sub_;
};

std::string formatAddress(ZMQProtocol protocol, const std::string& address) {
  switch (protocol) {
    case IPC: return std::format("ipc:///tmp/{}.ipc", address);
    case INPROC: return std::format("inproc://{}", address);
    default: throw std::invalid_argument("Invalid ZMQProtocol");
  }
}

ZMQProtocol parseZMQProtocol(const std::string& protocol_str) {
  if (protocol_str == "ipc") {
    return IPC;
  }

  if (protocol_str == "inproc") {
    return INPROC;
  }

  throw std::invalid_argument("Invalid ZMQProtocol");
}

using robocin::ThreadPool;

static constexpr std::string_view kTopic = "robocin";

int number_of_modules;
std::atomic<int> running_modules{0};
std::atomic<bool> all_publishers_running{false};

struct ModuleInfo {
  int id;
  float time_to_wait_ms = 0.0F;

  std::unique_ptr<SubSocket> sub;
  std::unique_ptr<PubSocket> pub;

  std::mutex mutex;
  std::condition_variable cv;
  std::vector<ZmqDatagram> packages;

  ThreadPool thread_pool = ThreadPool(1);
};

void subscriberRun(ModuleInfo& info) {
  while (true) {
    {
      std::lock_guard lock(info.mutex);

      do {
        while (true) {
          auto message = info.sub->receive();
          if (message.topic.empty()) {
            break;
          }
          info.packages.push_back(message);
        }
      } while (info.packages.empty());
    }

    info.cv.notify_one();
  }
}

SubSocket makeSubscriberSocket(int id, ZMQProtocol protocol);
PubSocket makePublisherSocket(int id, ZMQProtocol protocol);
void makeModule(int id, float time_to_wait_ms, ZMQProtocol protocol);
Frame mapWrapperPacketToFrame(const SSL_WrapperPacket& packet);
void mockedSleep(float time_to_wait_ms);

Frame parseMessage(std::string message, int id) {
  if (id == 0) {
    SSL_WrapperPacket packet;
    packet.ParseFromString(message);

    Frame frame = mapWrapperPacketToFrame(packet);
    return frame;
  }
  Frame frame;
  frame.ParseFromString(message);
  return frame;
}

void publisherRun(ModuleInfo& info) {
  auto unsaved_frames = std::make_unique<std::vector<Frame>>();
  unsaved_frames->reserve(2048);

  while (true) {
    std::optional<Frame> processed_frame;

    std::vector<ZmqDatagram> local_packages;
    {
      std::unique_lock lock(info.mutex);
      info.cv.wait(lock, [&] { return !info.packages.empty(); });

      local_packages.swap(info.packages);
    }

    for (const auto& [_, message] : local_packages) {
      // std::cout << std::format("sending at module {}.", info.id) << std::endl;
      mockedSleep(info.time_to_wait_ms);

      processed_frame = parseMessage(message, info.id);
      auto message_parsed = processed_frame->SerializeAsString();

      info.pub->send(kTopic, message_parsed);
    }
  }
}

int main(int argc, char* argv[]) {
  // argv[0] is relative binary name.
  // argv[1] is the ZMQ protocol to be used between modules.
  // args[i] is how many milliseconds the i-th module should wait.
  assert(argc >= 3);

  ZMQProtocol protocol;
  try {
    protocol = parseZMQProtocol(argv[1]);
  } catch (const std::invalid_argument&) {
    std::cerr << "Invalid ZMQ protocol. Use 'ipc' or 'inproc'." << std::endl;
    return 1;
  }

  std::span times_to_wait{argv + 2, argv + argc};

  number_of_modules = times_to_wait.size();
  std::unique_ptr<std::jthread> threads[number_of_modules];
  for (int index = 0; index < number_of_modules; index++) {
    threads[index] = std::make_unique<std::jthread>(makeModule,
                                                    index,
                                                    std::stof(times_to_wait[index]),
                                                    protocol);
  }
  return 0;
}

void makeModule(int id, float time_to_wait_ms, ZMQProtocol protocol) {

  // std::cout << std::format("Module {} is running and waiting {} ms.!", id, time_to_wait_ms)
  //           << std::endl;

  ModuleInfo info;
  info.id = id;
  info.time_to_wait_ms = time_to_wait_ms;
  info.sub = std::make_unique<SubSocket>(makeSubscriberSocket(id, protocol));
  info.pub = std::make_unique<PubSocket>(makePublisherSocket(id, protocol));

  std::jthread published_thread(publisherRun, std::ref(info));

  running_modules++;
  if (running_modules >= number_of_modules) {
    // std::cout << "All subscribers are running." << std::endl;
    all_publishers_running = true;
    all_publishers_running.notify_all();
  }

  all_publishers_running.wait(false);
  std::jthread subscriber_thread(subscriberRun, std::ref(info));
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
SubSocket makeSubscriberSocket(int id, ZMQProtocol protocol) {
  SubSocket sub;

  if (id == 0) {
    static constexpr std::string_view k3rdPartyAddress = "ipc:///tmp/gateway-pub-th-parties.ipc";
    static constexpr std::string_view k3rdPartyTopic = "vision-third-party";

    sub.connect(k3rdPartyAddress, k3rdPartyTopic);
    // std::cout << std::format("Module {} receiving from third party.", id) << std::endl;
  } else {
    std::string address = formatAddress(protocol, std::format("channel{}", id - 1));
    // std::cout << std::format("Module {} receiving from Module {}.", id, id - 1) << std::endl;
    sub.connect(address, kTopic);
  }

  return sub;
}

PubSocket makePublisherSocket(int id, ZMQProtocol protocol) {
  PubSocket pub;

  if (id == number_of_modules - 1) {
    std::string address = std::format("ipc:///tmp/channel{}.ipc", id);
    // std::cout << std::format("Module {} bind at: '{}'", id, address) << std::endl;
    pub.bind(address);
  } else {
    std::string address = formatAddress(protocol, std::format("channel{}", id));
    // std::cout << std::format("Module {} bind at: '{}'", id, address) << std::endl;
    pub.bind(address);
  }

  return pub;
}

void mockedSleep(float time_to_wait_ms) {
  std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(time_to_wait_ms));
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

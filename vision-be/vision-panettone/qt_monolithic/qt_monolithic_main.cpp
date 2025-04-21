#include "qt_monolithic/utility.h"

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
#include <QCoreApplication>
#include <random>
#include <robocin/concurrency/thread_pool.h>
#include <robocin/network/zmq_publisher_socket.h>
#include <robocin/network/zmq_request_reply_socket.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <span>
#include <string>
#include <thread>
#include <vector>

using protocols::vision::Field;
using protocols::vision::Frame;
using robocin::ZmqDatagram;
using robocin::ZmqPublisherSocket;
using robocin::ZmqSubscriberSocket;
using vision::qt_monolithic::AModule;
using vision::qt_monolithic::GetDateTimeNow;

std::unique_ptr<ZmqPublisherSocket> pub;
ZmqPublisherSocket makePublisherSocket(int id) {
  ZmqPublisherSocket pub;

  std::string address = std::format("ipc:///tmp/channel{}.ipc", id);
  // std::cout << std::format("Service {} bind at: '{}'", id, address) << std::endl;
  pub.bind(address);

  return pub;
}

using QtModule = AModule;

Frame GetMockedFrame(int frame_id) {
  Frame frame;
  frame.mutable_properties()->set_serial_id(frame_id);

  Field& field = *frame.mutable_field();
  field.set_length(9000);
  field.set_width(6000);
  field.set_goal_depth(180);
  field.set_goal_width(1000);
  field.set_penalty_area_depth(1000);
  field.set_penalty_area_width(2000);
  field.set_boundary_width(300);
  field.set_goal_center_to_penalty_mark(6000);

  for (int i = 0; i < 11; ++i) {
    protocols::vision::Robot& robot = *frame.add_robots();
    robot.mutable_robot_id()->set_number(i);
    robot.mutable_robot_id()->set_color(
        protocols::common::RobotId_Color::RobotId_Color_COLOR_YELLOW);
    robot.set_angle(0);
    robot.set_confidence(1.0);
    robot.mutable_position()->set_x((i * 9000) / 11);
    robot.mutable_position()->set_y(0);
    robot.mutable_velocity()->set_x(0);
    robot.mutable_velocity()->set_y(0);
    robot.set_angular_velocity(0);
  }

  for (int i = 0; i < 11; ++i) {
    protocols::vision::Robot& robot = *frame.add_robots();
    robot.mutable_robot_id()->set_number(i);
    robot.mutable_robot_id()->set_color(protocols::common::RobotId_Color::RobotId_Color_COLOR_BLUE);

    robot.set_angle(0);
    robot.set_confidence(1.0);

    robot.mutable_position()->set_x((i * 9000) / 11);
    robot.mutable_position()->set_y(0);

    robot.mutable_velocity()->set_x(0);
    robot.mutable_velocity()->set_y(0);
    robot.set_angular_velocity(0);
  }

  return frame;
}

class ManualFirstModule : public AModule {
 public:
  ManualFirstModule(int id, float sleep_duration_ms) : AModule(id, sleep_duration_ms) {
    assert(id == 0);
  }

 protected:
  void ParallelRun() override {
    int frame_id = 0;
    while (true) {
      MockedSleep();

      // std::cout << std::format("\n[{}] Sending at Module {}.", GetDateTimeNow(), id_) <<
      // std::endl;
      ISendMessage({.topic = std::string(kTopic),
                    .message = GetMockedFrame(frame_id++).SerializeAsString()});
    }
  }
};

class FirstModule : public AModule {
  static constexpr std::string_view k3rdPartyAddress = "ipc:///tmp/gateway-pub-th-parties.ipc";
  static constexpr std::string_view k3rdPartyTopic = "vision-third-party";

 public:
  FirstModule(int id, float sleep_duration_ms) : AModule(id, sleep_duration_ms) { assert(id == 0); }

 protected:
  void ParallelRun() override {
    zmq_subscriber_socket_.connect(k3rdPartyAddress, std::span{&k3rdPartyTopic, 1});
    // std::cout << std::format("Service {} receiving from third party.", id_) << std::endl;

    // std::this_thread::sleep_for(std::chrono::duration<int>(1)); // 1s sleep before start.

    while (true) {
      auto message = zmq_subscriber_socket_.receive();
      if (message.topic.empty()) {
        continue;
      }

      MockedSleep();
      ISendMessage({.topic = std::string(kTopic), .message = MakeNextMessage(message.message)});
    }
  }

 private:
  ZmqSubscriberSocket zmq_subscriber_socket_;
};

class LastModule : public AModule {
 public:
  LastModule(int id, float sleep_duration_ms) : AModule(id, sleep_duration_ms) {}

  void LastSendMessage(const ZmqDatagram& message) {
    if (message.topic.empty()) {
      // std::cerr << "Topic is empty!" << std::endl;
      return;
    }
    if (message.message.empty()) {
      // std::cerr << "Message is empty!" << std::endl;
      return;
    }

    // std::cout << "Sending Using ZMQ..." << std::endl;
    pub->send(kTopic, message.message);
    // std::cout << "Sent!" << std::endl;
  }

  void ParallelRun() override {
    // std::string address = std::format("ipc:///tmp/channel{}.ipc", id_);
    // zmq_publisher_socket_.bind(address);
    // std::cout << std::format("LastModule bind at: '{}'", address) << std::endl;

    while (true) {
      std::vector<ZmqDatagram> packages;
      {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [&] { return !shared_packages_.empty(); });

        packages.swap(shared_packages_);
      }

      for (const auto& [_, message] : packages) {
        MockedSleep();

        LastSendMessage({.topic = std::string(kTopic), .message = MakeNextMessage(message)});
      }
    }
  }

 private:
  // ZmqPublisherSocket zmq_publisher_socket_;
};

// cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ && cmake --build build

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  std::unique_ptr<AModule> first_module
      = std::make_unique<FirstModule>(/*id=*/0, /*sleep_duration_ms=*/3.0F);
  std::unique_ptr<AModule> second_module
      = std::make_unique<QtModule>(/*id=*/1, /*sleep_duration_ms=*/2.5F);
  std::unique_ptr<AModule> third_module
      = std::make_unique<QtModule>(/*id=*/2, /*sleep_duration_ms=*/0.5F);
  std::unique_ptr<AModule> fourth_module
      = std::make_unique<QtModule>(/*id=*/3, /*sleep_duration_ms=*/9.0F);

  std::unique_ptr<AModule> last_module
      = std::make_unique<LastModule>(/*id=*/4, /*sleep_duration_ms=*/0.5F);
  pub = std::make_unique<ZmqPublisherSocket>(makePublisherSocket(4));

  second_module->Connect<FirstModule, QtModule>(*first_module);
  third_module->Connect<QtModule, QtModule>(*second_module);
  fourth_module->Connect<QtModule, QtModule>(*third_module);
  last_module->Connect<QtModule, LastModule>(*fourth_module);

  std::jthread first_thread([&]() { first_module->ParallelRun(); });
  // first_module->Run();

  std::jthread second_thread([&]() { second_module->ParallelRun(); });
  // second_module->Run();

  std::jthread third_thread([&]() { third_module->ParallelRun(); });
  // third_module->Run();

  std::jthread fourth_thread([&]() { fourth_module->ParallelRun(); });
  // fourth_module->Run();

  std::jthread last_thread([&]() { last_module->ParallelRun(); });
  // last_module->Run();

  //   auto modules = std::to_array<std::unique_ptr<AModule>>({
  //       std::make_unique<FirstModule>(/*id=*/0, /*sleep_duration_ms=*/3.0F),
  //       std::make_unique<QtModule>(/*id=*/1, /*sleep_duration_ms=*/2.5F),
  //       std::make_unique<QtModule>(/*id=*/2, /*sleep_duration_ms=*/0.5F),
  //       std::make_unique<QtModule>(/*id=*/3, /*sleep_duration_ms=*/9.0F),
  //   });

  // for (int i = 1; i < modules.size(); ++i) {
  //   modules[i]->Connect(*modules[i - 1]);
  // }

  // last_module.Connect(*modules.back());

  // for (std::unique_ptr<AModule>& m : modules) {
  //   m->Run();
  // }

  return 0;
}

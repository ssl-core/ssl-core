#include "iframe_repository.h"
#include "vision/db/irepository_factory.h"
#include "vision/db/repository_factory_mapping.h"

#include <absl/synchronization/mutex.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <condition_variable>
#include <google/protobuf/duration.pb.h>
#include <google/protobuf/text_format.h>
#include <memory>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/vision/frame.pb.h>
#include <robocin/concurrency/thread_pool.h>
#include <robocin/network/zmq_publisher_socket.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <span>
#include <string>
#include <thread>

using protocols::third_party::detection::SSL_WrapperPacket;

using protocols::vision::Field;
using protocols::vision::Frame;

using robocin::ThreadPool;
using robocin::ZmqDatagram;

using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

std::mutex mutex;
std::condition_variable cv;
std::vector<ZmqDatagram> packages;

const uint64_t kFPS = 80;

uint64_t frame_id = 1;

// saves a frame to the database.
void saveToDatabase(IFrameRepository& repository, const Frame& frame) { repository.save(frame); }

Frame createMockedFrame() {
  Frame frame;
  frame.mutable_properties()->set_serial_id(frame_id++);

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

    robot.mutable_position()->set_x((i * 9000) / 11);
    robot.mutable_position()->set_y(0);
  }

  for (int i = 0; i < 11; ++i) {
    protocols::vision::Robot& robot = *frame.add_robots();
    robot.mutable_robot_id()->set_number(i);
    robot.mutable_robot_id()->set_color(protocols::common::RobotId_Color::RobotId_Color_COLOR_BLUE);

    robot.mutable_position()->set_x((i * 9000) / 11);
    robot.mutable_position()->set_y(0);
  }

  return frame;
}

void subscriberRun() {
  std::cout << "Starting subscriberRun..." << "\n";

  robocin::ZmqSubscriberSocket vision_third_party_socket{};
  const std::string_view kAddress = "ipc:///tmp/gateway-pub-th-parties.ipc";
  const std::string_view kTopic = "vision-third-party";
  vision_third_party_socket.connect(kAddress, std::span{&kTopic, 1});

  while (true) {
    {
      std::lock_guard lock(mutex);

      if (auto message = vision_third_party_socket.receive(); !message.message.empty()) {
        packages.push_back(message);
      }
    }

    cv.notify_one();
  }
}

void publisherRun() {
  std::cout << "Starting publisherRun..." << "\n";

  const std::string_view kVisionMessageTopic = "vision-third-party";

  robocin::ZmqPublisherSocket vision_publisher;
  vision_publisher.bind("ipc:///tmp/vision-async.ipc");

  // TODO($ISSUE_N): Move the database management to a class.
  // ThreadPool thread_pool(4);

  // std::cout << "Creating factory and frame repository." << "\n";
  // const auto kFactory = RepositoryFactoryMapping{}[RepositoryType::MongoDb];

  // std::unique_ptr<IFrameRepository> frame_repository = kFactory->createFrameRepository();
  // std::future<bool> connection_status = frame_repository->connect();

  // if (connection_status.wait(); connection_status.get()) {
  //   std::cout << "Connected to the database." << "\n";
  // } else {
  //   std::cout << "Failed to connect to the database." << "\n";
  //   return;
  // }

  // Receive datagrams.
  while (true) {
    std::vector<ZmqDatagram> datagrams;
    {
      std::unique_lock lock(mutex);
      cv.wait(lock, [&] { return !packages.empty(); });

      datagrams.swap(packages);
    }

    // TODO($ISSUE_N): Move this workflow to a DatagramHandler class.
    for (auto& datagram : datagrams) { // 1 frame == 2 pacotes, normalmente.
      auto topic = datagram.topic;
      if (topic == kVisionMessageTopic) {
        SSL_WrapperPacket detection;
        detection.ParseFromString(datagram.message);
        Frame frame = createMockedFrame();

        std::string message;
        frame.SerializeToString(&message);
        vision_publisher.send("frame", message);
        std::cout << "SENDING FRAME ON VISION PUB\n";
        // thread_pool.enqueue(saveToDatabase, std::ref(*frame_repository), std::cref(frame));
      } else {
        std::cout << "Unexpected topic for ZmqDatagram. Expect " << kVisionMessageTopic << " got "
                  << topic << " instead.\n";
      }

      vision_publisher.send("frame", "VISION MESSAGE");
    }
  }
}

int main(int argc, char* argv[]) {
  std::span args{argv + 1, argv + argc - 1};

  std::jthread publisher_thread(publisherRun);
  std::jthread subscriber_thread(subscriberRun);

  return 0;
}

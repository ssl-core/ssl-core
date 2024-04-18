#include "iframe_repository.h"
#include "vision/db/irepository_factory.h"
#include "vision/db/repository_factory_mapping.h"

#include <absl/synchronization/mutex.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <condition_variable>
#include <cstdlib>
#include <google/protobuf/duration.pb.h>
#include <google/protobuf/text_format.h>
#include <iostream>
#include <memory>
#include <mutex>
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

using protocols::third_party::detection::SSL_WrapperPacket;

using protocols::vision::Field;
using protocols::vision::Frame;

using robocin::ThreadPool;
using robocin::ZmqDatagram;

using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

const uint64_t kFPS = 80;
uint64_t frame_id = 1;

const auto kFactory = RepositoryFactoryMapping{}[RepositoryType::MongoDb];
std::unique_ptr<IFrameRepository> frame_repository = kFactory->createFrameRepository();

std::mutex mutex_db;
// Database:

// saves a frame to the database.
void saveToDatabase(IFrameRepository& repository, const Frame& frame) { repository.save(frame); }

// fetches a frame range from the database.
std::vector<Frame> findRangeFromDatabase(IFrameRepository& repository,
                                     const int64_t& key_lower_bound,
                                     const int64_t& key_upper_bound) {
  return repository.findRange(key_lower_bound, key_upper_bound);
}

// Helpers:

Frame createMockedFrame() {
  std::cout << "Creating mocked frame..." << std::endl;
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

static constexpr std::string_view kThirdPartyAddress = "ipc:///tmp/gateway-pub-th-parties.ipc";
static constexpr std::string_view kVisionMessageTopic = "vision-third-party";
static constexpr std::string_view kVisionPublisherAddress = "ipc:///tmp/vision-async.ipc";

ThreadPool thread_pool(4);

std::mutex mutex;
std::condition_variable cv;
std::vector<ZmqDatagram> packages;

void subscriberRun() {
  std::cout << "Subscriber thread running..." << std::endl;
  robocin::ZmqSubscriberSocket vision_third_party_socket{};
  vision_third_party_socket.connect(kThirdPartyAddress, std::span{&kVisionMessageTopic, 1});

  uint64_t total_msgs_received = 0;

  while (true) {
    {
      std::lock_guard lock(mutex);

      while (true) {
        auto message = vision_third_party_socket.receive();
        if (message.message.empty()) {
          break;
        }
        // std::cout << std::format("received message!, total: {}", total_msgs_received++) <<
        // std::endl;
        packages.push_back(message);
      }
    }

    cv.notify_one();
  }
}

void publisherRun() {
  std::cout << "Publisher thread running..." << std::endl;
  robocin::ZmqPublisherSocket vision_publisher;
  vision_publisher.bind(kVisionPublisherAddress);

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
        {
          std::lock_guard<std::mutex> lock(mutex_db);
          Frame frame = createMockedFrame();

          std::string message;
          frame.SerializeToString(&message);
          vision_publisher.send("frame", message);
          thread_pool.enqueue(saveToDatabase, std::ref(*frame_repository), std::cref(frame));
        }
      } else {
        std::cout << std::format("unexpected topic for ZmqDatagram: expect {}, got: {} instead.",
                                 kVisionMessageTopic,
                                 topic)
                  << std::endl;
      }
    }
  }
}

static constexpr std::string_view kReplyAddress = "ipc:///tmp/vision-sync.ipc";
void databaseHandlerRun() {
  std::cout << "Database thread running..." << std::endl;
  robocin::ZmqReplySocket vision_reply_socket{};
  vision_reply_socket.bind(kReplyAddress);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int64_t> distribution(1, 6);

  // Receive sync request.
  while (true) {
    if (auto request = vision_reply_socket.receive(); !request.empty()) {
      std::cout << "GetVisionChunk on VisionMS..." << std::endl;
      int64_t first_key = distribution(gen);
      int64_t second_key = distribution(gen);
      auto [lower, upper] = std::minmax(first_key, second_key);

      auto range
          = thread_pool.enqueue(findRangeFromDatabase, std::ref(*frame_repository), lower,
          upper);

      /*
      message ChunkResponseHeader {
        google.protobuf.Duration request_start = 1;
        uint32 chunk_id = 2;
        uint32 n_chunks = 3;

        google.protobuf.Duration max_duration = 4;
      }

      message GetVisionChunkResponse {
        ChunkResponseHeader header = 1;
        repeated vision.Frame payloads = 2;
      }
      */
      protocols::ui::GetVisionChunkResponse response;
      protocols::ui::ChunkResponseHeader &header = *response.mutable_header();
      header.mutable_request_start()->set_seconds(0);
      header.set_chunk_id(1);
      header.set_n_chunks(1);
      header.mutable_max_duration()->set_seconds(0);

      for (auto& frame : range.get()) {
        response.add_payloads()->CopyFrom(frame);
      }

      std::string serialized_response;
      response.SerializeToString(&serialized_response);
      vision_reply_socket.send(serialized_response);
    }
  }
}

int main() {
  std::cout << "Vision is runnning!" << std::endl;

  // TODO($ISSUE_N): Move the database management to a class.
  std::cout << "Creating factory and frame repository." << std::endl;

  // std::future<bool> connection_status = frame_repository->connect();

  // if (connection_status.wait(); connection_status.get()) {
  //   std::cout << "Connected to the database." << std::endl;
  // } else {
  //   std::cout << "Failed to connect to the database." << std::endl;
  //   return -1;
  // }

  std::jthread publisher_thread(publisherRun);
  std::jthread subscriber_thread(subscriberRun);
  std::jthread database_thread(databaseHandlerRun);

  return 0;
}

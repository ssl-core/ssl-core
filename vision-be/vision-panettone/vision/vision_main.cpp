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
#include <protocols/ui/replay.pb.h>
#include <protocols/vision/frame.pb.h>
#include <robocin/concurrency/thread_pool.h>
#include <robocin/network/zmq_publisher_socket.h>
#include <robocin/network/zmq_request_reply_socket.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <span>
#include <string>
#include <thread>

using ::google::protobuf::Duration;
using protocols::third_party::detection::SSL_WrapperPacket;

using protocols::ui::ReplayRequest;
using protocols::vision::Frame;

using robocin::ThreadPool;
using robocin::ZmqDatagram;
using robocin::ZmqReplySocket;

using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

std::mutex mutex;
std::condition_variable cv;
std::vector<ZmqDatagram> packages;

const uint64_t kFPS = 80;

// // saves a frame to the database.
// void saveToDatabase(IFrameRepository& repository, const Frame& frame) { repository.save(frame); }

// // fetches a frame from the database according its key.
// std::optional<Frame> fetchFromDatabase(IFrameRepository& repository, int64_t key) {
//   return repository.find(key);
// }

// // fetches a range of frames from the database according to the key lower and upper bounds.
// std::vector<Frame> fetchRangeFromDatabase(IFrameRepository& repository,
//                                           int64_t key_lower_bound,
//                                           int64_t key_upper_bound) {
//   return repository.findRange(key_lower_bound, key_upper_bound);
// }

// void subscriberRun() {
//   std::unique_ptr<robocin::ZmqSubscriberSocket> gateway_sub_vision_socket;
//   gateway_sub_vision_socket->connect("ipc:///tmp/gateway-pub-th-parties.ipc",
//   "vision-third-party");

//   while (true) {
//     {
//       std::lock_guard lock(mutex);

//       if (auto message = gateway_sub_vision_socket->receive(); !message.message.empty()) {
//         packages.push_back(message);
//       }
//     }

//     cv.notify_one();
//   }
// }

// void publisherRun() {
//   std::unique_ptr<robocin::ZmqPublisherSocket> vision_publisher;
//   vision_publisher->bind("ipc:///tmp/vision-async.ipc");

//   std::unique_ptr<robocin::ZmqReplySocket> vision_reply;
//   vision_reply->bind("ipc:///tmp/vision-sync.ipc");

//   // // TODO($ISSUE_N): Move the database management to a class.
//   // ThreadPool thread_pool(4);

//   // std::cout << "Creating factory and frame repository" << "\n";
//   // const auto kFactory = RepositoryFactoryMapping{}[RepositoryType::MongoDb];

//   // std::unique_ptr<IFrameRepository> frame_repository = kFactory->createFrameRepository();
//   // std::future<bool> connection_status = frame_repository->connect();

//   // if (connection_status.wait(); connection_status.get()) {
//   //   std::cout << "Connected to the database." << "\n";
//   // } else {
//   //   std::cout << "Failed to connect to the database." << "\n";
//   //   return;
//   // }

//   while (true) {
//     std::vector<ZmqDatagram> datagrams;
//     {
//       std::unique_lock lock(mutex);
//       cv.wait(lock, [&] { return !packages.empty(); });

//       datagrams.swap(packages);
//     }

//     // TODO($ISSUE_N): Move this workflow to a DatagramHandler class.
//     for (auto& datagram : datagrams) { // 1 frame == 2 pacotes, normalmente.
//       auto topic = datagram.topic;
//       if (topic == "detection") {
//         SSL_WrapperPacket detection;
//         detection.ParseFromString(datagram.message);

//         auto process_raw_frame = []() { return Frame{}; };

//         // TODO($ISSUE_N): Move this process to a FrameProcessor class with vision filters.
//         // processa o wrapper.
//         // extrapola ou manda com o ultimo processado.
//         Frame frame = process_raw_frame(/*detection*/);

//         std::string message;
//         frame.SerializeToString(&message);
//         vision_publisher->send("frame", message);
//         // thread_pool.enqueue(saveToDatabase, std::ref(*frame_repository), std::cref(frame));
//       } else if (topic == "replay") {
//         // tem o Duration, que seria o minuto do jogo a partir do inicio que a gente quer.
//         // precisa mapear em key_lower_bound.
//         // start = 1 second;
//         // ReplayRequest request;
//         // request.ParseFromString(datagram.message);
//         // Duration start = request.start();
//         // int64_t key_lower_bound = start.seconds() * kFPS;
//         // const int64_t kNumberOfFramesPerRange = 180;
//         // thread_pool.enqueue(fetchRangeFromDatabase,
//         //                     std::ref(*frame_repository),
//         //                     key_lower_bound,
//         //                     key_lower_bound + kNumberOfFramesPerRange);

//         vision_reply->send(datagram.message);
//       }
//     }
//     // publisher->send("robocin", PubSubMode::Wait, packages.back());
//   }
// }

// int main(int argc, char* argv[]) {
//   std::span args{argv + 1, argv + argc - 1};

//   // std::jthread subscriber_thread(subscriberRun);
//   std::string text = R"pb(detection {
//   frame_number: 840
//   t_capture: 14.016946999999991
//   t_sent: 14.016946999999991
//   camera_id: 3
//   balls {
//     confidence: 0.965216219
//     x: 0
//     y: 0
//     pixel_x: 0
//     pixel_y: 0
//   }
//   robots_blue {
//     confidence: 1
//     robot_id: 0
//     x: -1499.97278
//     y: 1120
//     orientation: -0
//     pixel_x: -1499.97278
//     pixel_y: 1120
//   }
// })pb";
//   SSL_WrapperPacket packet;
//   google::protobuf::TextFormat::ParseFromString(text, &packet);
//   std::cout << "After ParseFromString" << packet.DebugString() << "\n";

//   std::string serialized;
//   packet.SerializeToString(&serialized);
//   packages = {{"detection", serialized}, {"detection", serialized}, {"detection", serialized}};
//   std::jthread publisher_thread(publisherRun);

//   return 0;
// }

int main() {
  std::string const kVisionRepService = "ipc:///tmp/vision-sync.ipc";
  ZmqReplySocket reply;
  reply.bind(kVisionRepService);

  while (true) {
    auto message = reply.receive();
    std::cout << "Receive " << message << "\n";
    reply.send(std::string_view{"Response."});
  }
}

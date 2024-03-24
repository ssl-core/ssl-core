#include "iframe_repository.h"
#include "vision/db/irepository_factory.h"
#include "vision/db/repository_factory_mapping.h"
#include "vision/network/ireceiver_communication.h"
#include "vision/network/isender_communication.h"
#include "vision/network/zmq_receiver_communication.h"
#include "vision/network/zmq_sender_communication.h"

#include <absl/synchronization/mutex.h>
#include <absl/time/clock.h>
#include <absl/time/time.h>
#include <condition_variable>
#include <memory>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/vision/frame.pb.h>
#include <robocin/concurrency/thread_pool.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <span>
#include <thread>

using protocols::third_party::detection::SSL_WrapperPacket;
using protocols::vision::Frame;

using robocin::ZmqDatagram;
using vision::IReceiverCommunication;
using vision::ISenderCommunication;
using vision::ZMQReceiverCommunication;
using vision::ZMQSenderCommunication;

using robocin::ThreadPool;
using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

std::mutex mutex;
std::condition_variable cv;
std::vector<ZmqDatagram> packages;

// saves a frame to the database.
void saveToDatabase(IFrameRepository& repository, const Frame& frame) { repository.save(frame); }

// fetches a frame from the database according its key.
std::optional<Frame> fetchFromDatabase(IFrameRepository& repository, int64_t key) {
  return repository.find(key);
}

void subscriberRun() {
  std::unique_ptr<IReceiverCommunication> receiver = std::make_unique<ZMQReceiverCommunication>();
  receiver->connect("");

  while (true) {
    {
      std::lock_guard lock(mutex);

      if (auto message = receiver->receive(); !message.message.empty()) {
        packages.push_back(message);
      }
    }

    cv.notify_one();
  }
}

void publisherRun() {
  std::unique_ptr<ISenderCommunication> sender = std::make_unique<ZMQSenderCommunication>();
  sender->bind("", "");

  // TODO($ISSUE_N): Move the database management to a class.
  ThreadPool thread_pool(4);

  std::cout << "Creating factory and frame repository" << "\n";
  const auto kFactory = RepositoryFactoryMapping{}[RepositoryType::MongoDb];

  std::unique_ptr<IFrameRepository> frame_repository = kFactory->createFrameRepository();
  std::future<bool> connection_status = frame_repository->connect();

  if (connection_status.wait(); connection_status.get()) {
    std::cout << "Connected to the database." << "\n";
  } else {
    std::cout << "Failed to connect to the database." << "\n";
    return;
  }

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
      if (topic == "detection") {
        SSL_WrapperPacket detection;
        detection.ParseFromString(datagram.message);

        auto process_raw_frame = []() { return Frame{}; };

        // TODO($ISSUE_N): Move this process to a FrameProcessor class with vision filters.
        // processa o wrapper.
        // extrapola ou manda com o ultimo processado.
        Frame frame = process_raw_frame(/*detection*/);

        sender->send(frame);
        thread_pool.enqueue(saveToDatabase, std::ref(*frame_repository), std::cref(frame));
      } else if (topic == "replay") {
        // get do ban
      }
    }

    // publisher->send("robocin", PubSubMode::Wait, packages.back());
  }
}

int main(int argc, char* argv[]) {
  std::span args{argv + 1, argv + argc - 1};

  std::jthread subscriber_thread(subscriberRun);
  std::jthread publisher_thread(publisherRun);

  return 0;
}

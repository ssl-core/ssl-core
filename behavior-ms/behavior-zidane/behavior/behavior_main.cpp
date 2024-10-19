#include "behavior/controller/consumer_controller.h"
#include "behavior/controller/producer_controller.h"

#include <memory>
#include <print>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_publisher_socket.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <robocin/parameters/parameters.h>
#include <robocin/third_party/adaptors/pb_time_util.h>
#include <robocin/wip/service_discovery/addresses.h>
#include <thread>

using behavior::BehaviorProcessor;
using behavior::ConsumerController;
using behavior::IBehaviorProcessor;
using behavior::IController;
using behavior::IMessageReceiver;
using behavior::IMessageSender;
using behavior::IPayloadMapper;
using behavior::MessageReceiver;
using behavior::MessageSender;
using behavior::Payload;
using behavior::PayloadMapper;
using behavior::ProducerController;
using ::robocin::ConditionVariableConcurrentQueue;
using ::robocin::IConcurrentQueue;
using ::robocin::ilog;
using ::robocin::IZmqPoller;
using ::robocin::IZmqPublisherSocket;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::object_ptr;
using ::robocin::ZmqPoller;
using ::robocin::ZmqPublisherSocket;
using ::robocin::ZmqSubscriberSocket;

namespace parameters = ::robocin::parameters;
namespace service_discovery = robocin::service_discovery;

std::unique_ptr<IMessageReceiver> makeMessageReceiver() {
  static constexpr std::array kPerceptionTopics = {
      service_discovery::kPerceptionDetectionTopic,
  };
  static constexpr std::array kDecisionTopics = {
      service_discovery::kDecisionTopic,
  };

  std::unique_ptr<IZmqSubscriberSocket> perception_socket = std::make_unique<ZmqSubscriberSocket>();
  perception_socket->connect(service_discovery::kPerceptionAddress, kPerceptionTopics);

  std::unique_ptr<IZmqSubscriberSocket> decision_socket = std::make_unique<ZmqSubscriberSocket>();
  decision_socket->connect(service_discovery::kDecisionAddress, kDecisionTopics);

  std::unique_ptr<IZmqPoller> zmq_poller = std::make_unique<ZmqPoller>();
  zmq_poller->push(*perception_socket);
  zmq_poller->push(*decision_socket);

  std::unique_ptr<IPayloadMapper> payload_mapper = std::make_unique<PayloadMapper>();

  return std::make_unique<MessageReceiver>(std::move(perception_socket),
                                           std::move(decision_socket),
                                           std::move(zmq_poller),
                                           std::move(payload_mapper));
}

std::unique_ptr<IController> makeProducer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ProducerController>(messages, makeMessageReceiver());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IBehaviorProcessor> makeBehaviorProcessor() {
  return std::make_unique<BehaviorProcessor>(std::make_unique<parameters::HandlerEngine>());
}

std::unique_ptr<IMessageSender> makeMessageSender() {
  std::unique_ptr<IZmqPublisherSocket> behavior_socket = std::make_unique<ZmqPublisherSocket>();
  behavior_socket->bind(service_discovery::kBehaviorAddress);

  return std::make_unique<MessageSender>(std::move(behavior_socket));
}

std::unique_ptr<IController> makeConsumer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ConsumerController>(messages,
                                              std::make_unique<parameters::HandlerEngine>(),
                                              makeBehaviorProcessor(),
                                              makeMessageSender());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  std::println("behavior-zidane is runnning!");

  std::unique_ptr<IConcurrentQueue<Payload>> messages
      = std::make_unique<ConditionVariableConcurrentQueue<Payload>>();

  std::unique_ptr<IController> producer_controller = makeProducer(messages);
  std::unique_ptr<IController> consumer_controller = makeConsumer(messages);

  std::jthread producer_thread(&IController::run, producer_controller.get());
  std::jthread consumer_thread(&IController::run, consumer_controller.get());

  return 0;
}

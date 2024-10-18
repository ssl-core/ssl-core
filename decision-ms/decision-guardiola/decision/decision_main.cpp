#include "decision/controller/producer_controller.h"
#include "decision/controller/consumer_controller.h"

#include <memory>
#include <print>
#include <robocin/detection_util/clock.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <robocin/parameters/parameters.h>
#include <robocin/wip/service_discovery/addresses.h>
#include <thread>

namespace parameters = ::robocin::parameters;
namespace detection_util = ::robocin::detection_util;
namespace service_discovery = robocin::service_discovery;

using decision::ConsumerController;
using decision::IController;
using decision::IMessageReceiver;
using decision::IMessageSender;
using decision::IPayloadMapper;
using decision::IDecisionProcessor;
using decision::MessageReceiver;
using decision::MessageSender;
using decision::Payload;
using decision::PayloadMapper;
using decision::ProducerController;
using decision::DecisionProcessor;
using ::robocin::ConditionVariableConcurrentQueue;
using ::robocin::IConcurrentQueue;
using ::robocin::IZmqPoller;
using ::robocin::IZmqPublisherSocket;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::object_ptr;
using ::robocin::ZmqPoller;
using ::robocin::ZmqPublisherSocket;
using ::robocin::ZmqSubscriberSocket;
using ::robocin::ilog;

std::unique_ptr<IMessageReceiver> makeMessageReceiver() {
  static constexpr std::array kPerceptionTopics = {
      service_discovery::kPerceptionDetectionTopic,
  };
  static constexpr std::array kRefereeTopics = {
      service_discovery::kRefereeGameStatusTopic,
  };

  std::unique_ptr<IZmqSubscriberSocket> referee_socket = std::make_unique<ZmqSubscriberSocket>();
  referee_socket->connect(service_discovery::kRefereeAddress, kRefereeTopics);

  std::unique_ptr<IZmqSubscriberSocket> perception_socket = std::make_unique<ZmqSubscriberSocket>();
  perception_socket->connect(service_discovery::kPerceptionAddress, kPerceptionTopics);

  std::unique_ptr<IZmqPoller> zmq_poller = std::make_unique<ZmqPoller>();
  zmq_poller->push(*referee_socket);
  zmq_poller->push(*perception_socket);

  std::unique_ptr<IPayloadMapper> payload_mapper = std::make_unique<PayloadMapper>();

  return std::make_unique<MessageReceiver>(std::move(perception_socket),
                                           std::move(referee_socket),
                                           std::move(zmq_poller),
                                           std::move(payload_mapper));
}

std::unique_ptr<IController> makeProducer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ProducerController>(messages, makeMessageReceiver());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IDecisionProcessor> makeDecisionProcessor() {
  return std::make_unique<DecisionProcessor>(
      std::make_unique<parameters::HandlerEngine>());
}

std::unique_ptr<IMessageSender> makeMessageSender() {
  std::unique_ptr<IZmqPublisherSocket> decision_socket = std::make_unique<ZmqPublisherSocket>();
  decision_socket->bind(service_discovery::kDecisionAddress);

  return std::make_unique<MessageSender>(std::move(decision_socket));
}

std::unique_ptr<IController> makeConsumer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ConsumerController>(messages,
                                              std::make_unique<parameters::HandlerEngine>(),
                                              makeDecisionProcessor(),
                                              makeMessageSender());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  std::println("decision-guardiola is running!");

  std::unique_ptr<IConcurrentQueue<Payload>> messages
      = std::make_unique<ConditionVariableConcurrentQueue<Payload>>();

  std::unique_ptr<IController> producer_controller = makeProducer(messages);
  std::unique_ptr<IController> consumer_controller = makeConsumer(messages);

  std::jthread producer_thread(&IController::run, producer_controller.get());
  std::jthread consumer_thread(&IController::run, consumer_controller.get());

  return 0;
}

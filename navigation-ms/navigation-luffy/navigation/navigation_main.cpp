#include "navigation/controller/consumer_controller.h"
#include "navigation/controller/producer_controller.h"

#include <memory>
#include <print>
#include <robocin/memory/object_ptr.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <robocin/parameters/parameters.h>
#include <robocin/wip/service_discovery/addresses.h>
#include <thread>

namespace parameters = ::robocin::parameters;
namespace service_discovery = robocin::service_discovery;

using navigation::ConsumerController;
using navigation::IController;
using navigation::IMessageReceiver;
using navigation::IMessageSender;
using navigation::INavigationProcessor;
using navigation::IPayloadMapper;
using navigation::MessageReceiver;
using navigation::MessageSender;
using navigation::MotionParser;
using navigation::NavigationProcessor;
using navigation::Payload;
using navigation::PayloadMapper;
using navigation::ProducerController;
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

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IMessageReceiver> makeMessageReceiver() {
  static constexpr std::array kBehaviorTopics = {
      service_discovery::kBehaviorUnificationTopic,
  };

  static constexpr std::array kPerceptionTopics = {
      service_discovery::kPerceptionDetectionTopic,
  };

  std::unique_ptr<IZmqSubscriberSocket> behavior_socket = std::make_unique<ZmqSubscriberSocket>();
  behavior_socket->connect(service_discovery::kBehaviorAddress, kBehaviorTopics);

  std::unique_ptr<IZmqSubscriberSocket> perception_socket = std::make_unique<ZmqSubscriberSocket>();
  perception_socket->connect(service_discovery::kPerceptionAddress, kPerceptionTopics);

  std::unique_ptr<IZmqPoller> zmq_poller = std::make_unique<ZmqPoller>();
  zmq_poller->push(*behavior_socket);
  zmq_poller->push(*perception_socket);

  std::unique_ptr<IPayloadMapper> payload_mapper = std::make_unique<PayloadMapper>();

  return std::make_unique<MessageReceiver>(std::move(behavior_socket),
                                           std::move(perception_socket),
                                           std::move(zmq_poller),
                                           std::move(payload_mapper));
}

std::unique_ptr<IController> makeProducer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ProducerController>(messages, makeMessageReceiver());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<INavigationProcessor> makeNavigationProcessor() {
  return std::make_unique<NavigationProcessor>(
      std::make_unique<MotionParser>()); // TODO: passar os parâmetros aqui
}

std::unique_ptr<IMessageSender> makeMessageSender() {
  std::unique_ptr<IZmqPublisherSocket> navigation_socket = std::make_unique<ZmqPublisherSocket>();
  navigation_socket->bind(service_discovery::kNavigationAddress);

  return std::make_unique<MessageSender>(std::move(navigation_socket));
}

std::unique_ptr<IController> makeConsumer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ConsumerController>(messages,
                                              makeNavigationProcessor(),
                                              makeMessageSender());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  std::println("navigation-luffy is runnning!");

  std::unique_ptr<IConcurrentQueue<Payload>> messages
      = std::make_unique<ConditionVariableConcurrentQueue<Payload>>();

  std::unique_ptr<IController> producer_controller = makeProducer(messages);
  std::unique_ptr<IController> consumer_controller = makeConsumer(messages);

  std::jthread producer_thread(&IController::run, producer_controller.get());
  std::jthread consumer_thread(&IController::run, consumer_controller.get());

  return 0;
}

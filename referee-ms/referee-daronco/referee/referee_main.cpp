#include "referee/controller/consumer_controller.h"
#include "referee/controller/producer_controller.h"

#include <memory>
#include <print>
#include <robocin/memory/object_ptr.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <robocin/wip/service_discovery/addresses.h>
#include <thread>

namespace service_discovery = robocin::service_discovery;

using referee::ConsumerController;
using referee::GameCommandMapper;
using referee::GameEventsMapper;
using referee::GameStageMapper;
using referee::GameStatusMapper;
using referee::IController;
using referee::IMessageReceiver;
using referee::IMessageSender;
using referee::IPayloadMapper;
using referee::IRefereeProcessor;
using referee::MessageReceiver;
using referee::MessageSender;
using referee::Payload;
using referee::PayloadMapper;
using referee::ProducerController;
using referee::RefereeProcessor;
using referee::TeamStatusMapper;
using ::robocin::ConditionVariableConcurrentQueue;
using ::robocin::IConcurrentQueue;
using ::robocin::IZmqPoller;
using ::robocin::IZmqPublisherSocket;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::object_ptr;
using ::robocin::ZmqPoller;
using ::robocin::ZmqPublisherSocket;
using ::robocin::ZmqSubscriberSocket;

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IMessageReceiver> makeMessageReceiver() {
  static constexpr std::array kPerceptionTopics = {
      service_discovery::kPerceptionDetectionTopic,
  };
  static constexpr std::array kGatewayTopics = {
      service_discovery::kGameControllerRefereeTopic,
  };

  std::unique_ptr<IZmqSubscriberSocket> gateway_socket = std::make_unique<ZmqSubscriberSocket>();
  gateway_socket->connect(service_discovery::kGatewayAddress, kGatewayTopics);

  std::unique_ptr<IZmqSubscriberSocket> perception_socket = std::make_unique<ZmqSubscriberSocket>();
  perception_socket->connect(service_discovery::kPerceptionAddress, kPerceptionTopics);

  std::unique_ptr<IZmqPoller> zmq_poller = std::make_unique<ZmqPoller>();
  zmq_poller->push(*gateway_socket);
  zmq_poller->push(*perception_socket);

  std::unique_ptr<IPayloadMapper> payload_mapper = std::make_unique<PayloadMapper>();

  return std::make_unique<MessageReceiver>(std::move(gateway_socket),
                                           std::move(perception_socket),
                                           std::move(zmq_poller),
                                           std::move(payload_mapper));
}

std::unique_ptr<IController> makeProducer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ProducerController>(messages, makeMessageReceiver());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IRefereeProcessor> makeRefereeProcessor() {
  return std::make_unique<RefereeProcessor>(
      std::make_unique<GameStatusMapper>(std::make_unique<TeamStatusMapper>(),
                                         std::make_unique<GameStageMapper>(),
                                         std::make_unique<GameCommandMapper>(),
                                         std::make_unique<GameEventsMapper>()));
}

std::unique_ptr<IMessageSender> makeMessageSender() {
  std::unique_ptr<IZmqPublisherSocket> referee_socket = std::make_unique<ZmqPublisherSocket>();
  referee_socket->bind(service_discovery::kRefereeAddress);

  return std::make_unique<MessageSender>(std::move(referee_socket));
}

std::unique_ptr<IController> makeConsumer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ConsumerController>(messages,
                                              makeRefereeProcessor(),
                                              makeMessageSender());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  std::println("referee-daronco is runnning!");

  std::unique_ptr<IConcurrentQueue<Payload>> messages
      = std::make_unique<ConditionVariableConcurrentQueue<Payload>>();

  std::unique_ptr<IController> producer_controller = makeProducer(messages);
  std::unique_ptr<IController> consumer_controller = makeConsumer(messages);

  std::jthread producer_thread(&IController::run, producer_controller.get());
  std::jthread consumer_thread(&IController::run, consumer_controller.get());

  return 0;
}

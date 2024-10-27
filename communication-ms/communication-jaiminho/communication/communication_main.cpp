#include "communication/controller/consumer_controller.h"
#include "communication/controller/producer_controller.h"

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
namespace service_discovery = robocin::service_discovery;

using communication::CommunicationProcessor;
using communication::ConsumerController;
using communication::ICommunicationProcessor;
using communication::IController;
using communication::IMessageReceiver;
using communication::IMessageSender;
using communication::IPayloadMapper;
using communication::MessageReceiver;
using communication::MessageSender;
using communication::Payload;
using communication::PayloadMapper;
using communication::ProducerController;
using communication::RobotCommandMapper;
using ::robocin::ConditionVariableConcurrentQueue;
using ::robocin::IConcurrentQueue;
using ::robocin::IUdpMulticastSocketSender;
using ::robocin::IZmqPoller;
using ::robocin::IZmqPublisherSocket;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::object_ptr;
using ::robocin::UdpMulticastSocketSender;
using ::robocin::ZmqPoller;
using ::robocin::ZmqPublisherSocket;
using ::robocin::ZmqSubscriberSocket;

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IMessageReceiver> makeMessageReceiver() {
  static constexpr std::array kNavigationTopics = {service_discovery::kNavigationOutputTopic};
  static constexpr std::array kGatewayTopics = {service_discovery::kGameControllerRefereeTopic};

  std::unique_ptr<IZmqSubscriberSocket> gateway_socket = std::make_unique<ZmqSubscriberSocket>();
  gateway_socket->connect(service_discovery::kGatewayAddress, kGatewayTopics);

  std::unique_ptr<IZmqSubscriberSocket> navigation_socket = std::make_unique<ZmqSubscriberSocket>();
  navigation_socket->connect(service_discovery::kNavigationAddress, kNavigationTopics);

  std::unique_ptr<IZmqPoller> zmq_poller = std::make_unique<ZmqPoller>();
  zmq_poller->push(*gateway_socket);
  zmq_poller->push(*navigation_socket);

  std::unique_ptr<IPayloadMapper> payload_mapper = std::make_unique<PayloadMapper>();

  return std::make_unique<MessageReceiver>(std::move(navigation_socket),
                                           std::move(gateway_socket),
                                           std::move(zmq_poller),
                                           std::move(payload_mapper));
}

std::unique_ptr<IController> makeProducer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ProducerController>(messages, makeMessageReceiver());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<ICommunicationProcessor> makeCommunicationProcessor() {
  return std::make_unique<CommunicationProcessor>(std::make_unique<parameters::HandlerEngine>(),
                                                  std::make_unique<RobotCommandMapper>());
}

std::unique_ptr<IMessageSender> makeMessageSender() {
  std::unique_ptr<IZmqPublisherSocket> communication_socket
      = std::make_unique<ZmqPublisherSocket>();
  communication_socket->bind(service_discovery::kCommunicationAddress);

  std::unique_ptr<IUdpMulticastSocketSender> robot_socket
      = std::make_unique<UdpMulticastSocketSender>();
  robot_socket->connect(service_discovery::kRobotAddress, service_discovery::kRobotPort);

  return std::make_unique<MessageSender>(std::move(communication_socket), std::move(robot_socket));
}

std::unique_ptr<IController> makeConsumer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ConsumerController>(messages,
                                              makeCommunicationProcessor(),
                                              makeMessageSender());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  std::println("communication-jaiminho is runnning!");

  std::unique_ptr<IConcurrentQueue<Payload>> messages
      = std::make_unique<ConditionVariableConcurrentQueue<Payload>>();

  std::unique_ptr<IController> producer_controller = makeProducer(messages);
  std::unique_ptr<IController> consumer_controller = makeConsumer(messages);

  std::jthread producer_thread(&IController::run, producer_controller.get());
  std::jthread consumer_thread(&IController::run, consumer_controller.get());

  return 0;
}

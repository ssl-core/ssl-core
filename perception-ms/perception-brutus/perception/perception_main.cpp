#include "perception/controller/consumer_controller.h"
#include "perception/controller/producer_controller.h"
#include "perception/messaging/receiver/payload.h"
#include "perception/processing/detection_processor.h"
#include "perception/processing/raw_detection/filters/ball_filter.h"
#include "perception/processing/raw_detection/filters/ball_filter_factory.h"
#include "perception/processing/raw_detection/filters/camera_filter_factory.h"
#include "perception/processing/raw_detection/filters/raw_detection_filter.h"
#include "perception/processing/raw_detection/filters/robot_filter.h"
#include "perception/processing/raw_detection/filters/robot_filter_factory.h"
#include "perception/processing/raw_detection/mappers/raw_detection_mapper.h"
#include "perception/processing/tracked_detection/mappers/tracked_detection_mapper.h"

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

namespace parameters = ::robocin::parameters;
namespace service_discovery = robocin::service_discovery;

using perception::BallFilter;
using perception::CameraFilter;
using perception::ConsumerController;
using perception::DetectionProcessor;
using perception::ICameraFilter;
using perception::IController;
using perception::IDetectionProcessor;
using perception::IMessageReceiver;
using perception::IMessageSender;
using perception::IPayloadMapper;
using perception::IRawDetectionFilter;
using perception::IRawDetectionMapper;
using perception::ITrackedDetectionFilter;
using perception::ITrackedDetectionMapper;
using perception::MessageReceiver;
using perception::MessageSender;
using perception::Payload;
using perception::PayloadMapper;
using perception::ProducerController;
using perception::RawDetectionFilter;
using perception::RawDetectionMapper;
using perception::RobotFilter;
using perception::TrackedDetectionFilter;
using perception::TrackedDetectionMapper;
using ::robocin::ConditionVariableConcurrentQueue;
using ::robocin::IConcurrentQueue;
using ::robocin::IZmqPoller;
using ::robocin::IZmqPublisherSocket;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::object_ptr;
using ::robocin::PbTimeUtil;
using ::robocin::ZmqPoller;
using ::robocin::ZmqPublisherSocket;
using ::robocin::ZmqSubscriberSocket;

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IMessageReceiver> makeMessageReceiver() {
  static constexpr std::array kGatewayTopics = {
      service_discovery::kRawDetectionTopic,
      service_discovery::kTrackedDetectionTopic,
  };

  std::unique_ptr<IZmqSubscriberSocket> gateway_socket = std::make_unique<ZmqSubscriberSocket>();
  gateway_socket->connect(service_discovery::kGatewayAddress, kGatewayTopics);

  std::unique_ptr<IZmqPoller> zmq_poller = std::make_unique<ZmqPoller>();
  zmq_poller->push(*gateway_socket);

  std::unique_ptr<IPayloadMapper> payload_mapper = std::make_unique<PayloadMapper>();

  return std::make_unique<MessageReceiver>(std::move(gateway_socket),
                                           std::move(zmq_poller),
                                           std::move(payload_mapper));
}

std::unique_ptr<IController> makeProducer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ProducerController>(messages, makeMessageReceiver());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<IRawDetectionMapper> makeRawDetectionMapper() {
  return std::make_unique<RawDetectionMapper>();
}

std::unique_ptr<IRawDetectionFilter> makeRawDetectionFilter() {
  std::unique_ptr<ICameraFilter::Factory> camera_filter_factory
      = std::make_unique<CameraFilter::Factory>(
          []() { return std::make_unique<BallFilter::Factory>(); },
          []() { return std::make_unique<RobotFilter::Factory>(); });

  return std::make_unique<RawDetectionFilter>(std::move(camera_filter_factory));
}

std::unique_ptr<ITrackedDetectionFilter> makeTrackedDetectionFilter() {
  std::unique_ptr<ITrackedDetectionMapper> tracked_detection_mapper
      = std::make_unique<TrackedDetectionMapper>();

  return std::make_unique<TrackedDetectionFilter>(std::move(tracked_detection_mapper));
}

std::unique_ptr<IDetectionProcessor> makeDetectionProcessor() {
  return std::make_unique<DetectionProcessor>(std::make_unique<PbTimeUtil>(),
                                              makeRawDetectionMapper(),
                                              makeRawDetectionFilter(),
                                              makeTrackedDetectionFilter());
}

std::unique_ptr<IMessageSender> makeMessageSender() {
  std::unique_ptr<IZmqPublisherSocket> detection_socket = std::make_unique<ZmqPublisherSocket>();
  detection_socket->bind(service_discovery::kPerceptionAddress);

  return std::make_unique<MessageSender>(std::move(detection_socket));
}

std::unique_ptr<IController> makeConsumer(object_ptr<IConcurrentQueue<Payload>> messages) {
  return std::make_unique<ConsumerController>(messages,
                                              std::make_unique<parameters::HandlerEngine>(),
                                              makeDetectionProcessor(),
                                              makeMessageSender());
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  std::println("perception-brutus is runnning!");

  std::unique_ptr<IConcurrentQueue<Payload>> messages
      = std::make_unique<ConditionVariableConcurrentQueue<Payload>>();

  std::unique_ptr<IController> producer_controller = makeProducer(messages);
  std::unique_ptr<IController> consumer_controller = makeConsumer(messages);

  std::jthread producer_thread(&IController::run, producer_controller.get());
  std::jthread consumer_thread(&IController::run, consumer_controller.get());

  return 0;
}

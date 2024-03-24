# ==========================
# = RECEIVER COMMUNICATION =
# ==========================
robocin_cpp_library(
  NAME ireceiver_communication
  HDRS ireceiver_communication.h
  SRCS ireceiver_communication.cpp
  DEPS common::network
)

robocin_cpp_library(
  NAME receiver_communication_factory
  HDRS receiver_communication_factory.h
  SRCS receiver_communication_factory.cpp
  DEPS ireceiver_communication
)

robocin_cpp_library(
  NAME receiver_communication_factory_mapping
  HDRS receiver_communication_factory_mapping.h
  SRCS receiver_communication_factory_mapping.cpp
  DEPS receiver_communication_factory
)

robocin_cpp_library(
  NAME zmq_receiver_communication
  HDRS zmq_receiver_communication.h
  SRCS zmq_receiver_communication.cpp
  DEPS ireceiver_communication cppzmq common::network
)

# ========================
# = SENDER COMMUNICATION =
# ========================
robocin_cpp_library(
  NAME isender_communication
  HDRS isender_communication.h
  SRCS isender_communication.cpp
  DEPS protocols::vision
)

robocin_cpp_library(
  NAME sender_communication_factory
  HDRS sender_communication_factory.h
  SRCS sender_communication_factory.cpp
  DEPS isender_communication
)

robocin_cpp_library(
  NAME sender_communication_factory_mapping
  HDRS sender_communication_factory_mapping.h
  SRCS sender_communication_factory_mapping.cpp
  DEPS sender_communication_factory
)

robocin_cpp_library(
  NAME zmq_sender_communication
  HDRS zmq_sender_communication.h
  SRCS zmq_sender_communication.cpp
  DEPS isender_communication cppzmq common::network
)

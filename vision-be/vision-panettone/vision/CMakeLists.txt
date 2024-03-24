robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  DEPS absl::time
       absl::synchronization
       absl::random_random
       absl::strings
       ireceiver_communication
       zmq_receiver_communication
       isender_communication
       zmq_sender_communication
       protocols::third_party
       protocols::ui
       common::thread_pool
       irepository_factory
       repository_factory_mapping
)

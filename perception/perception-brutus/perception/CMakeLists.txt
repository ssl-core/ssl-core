robocin_cpp_executable(
  NAME perception_main
  SRCS perception_main.cpp
  DEPS producer_controller consumer_controller vision_packets
)

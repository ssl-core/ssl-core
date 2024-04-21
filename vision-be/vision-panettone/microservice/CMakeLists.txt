robocin_cpp_executable(
  NAME microservice_main
  SRCS microservice_main.cpp
  DEPS common::network
       common::thread_pool
       protocols::third_party
       protocols::messages
       protocols::vision
       irepository_factory
       repository_factory_mapping
)

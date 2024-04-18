robocin_cpp_executable(
  NAME microservice_main
  SRCS microservice_main.cpp
  DEPS common::network
       protocols::vision
       protocols::third_party
)

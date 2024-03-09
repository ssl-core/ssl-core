robocin_cpp_library(
  NAME gateway 
  HDRS nlohmann/json.hpp service_discovery.h
  SRCS service_discovery.cpp 
  DEPS singleton
)

robocin_cpp_executable(
  NAME gateway_main
  SRCS gateway_main.cpp
  DEPS gateway controllers
)

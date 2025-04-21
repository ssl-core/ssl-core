set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

robocin_cpp_executable(
  NAME qt_monolithic_main
  HDRS utility.h
  SRCS utility.cpp
       qt_monolithic_main.cpp
  DEPS common::network
       common::thread_pool
       protocols::third_party
       protocols::messages
       protocols::vision
       Qt6::Core
)

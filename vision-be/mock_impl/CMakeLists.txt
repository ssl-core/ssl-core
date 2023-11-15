########################################################################################################################

cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../.cmake)

# define the project path to be used in the code
get_filename_component(ROBOCIN_PROJECT_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

include(project)

project(mock LANGUAGES CXX)

########################################################################################################################

# Find Protobuf, absl, gRPC from installed gRPC package
# It enable the following variables:
#   protobuf::libprotobuf                 the protobuf library
#   gRPC::grpc++_reflection               the gRPC++ reflection library
#   $<TARGET_FILE:protobuf::protoc>       the protobuf compiler
#   gRPC::grpc++                          the gRPC++ library
#   $<TARGET_FILE:gRPC::grpc_cpp_plugin>  the gRPC++ plugin executable
#   absl:: ...                            the abseil libraries (https://abseil.io/docs/cpp/guides)
# reference: https://github.com/grpc/grpc/blob/master/examples/cpp/cmake/common.cmake

# Find Threads
find_package(Threads REQUIRED)

# Find Protobuf installation
# Looks for protobuf cmake config files installed by Protobuf's cmake installation.
find_package(Protobuf CONFIG REQUIRED HINTS "/opt/grpc")
message(STATUS "Using Protobuf: ${Protobuf_VERSION}")

# Find gRPC installation
# Looks for gRPC cmake config files installed by gRPC's cmake installation.
find_package(absl CONFIG REQUIRED HINTS "/opt/grpc")
message(STATUS "Using absl: ${absl_VERSION}")

find_package(gRPC CONFIG REQUIRED HINTS "/opt/grpc")
message(STATUS "Using gRPC: ${gRPC_VERSION}")

########################################################################################################################

# Find cppzmq package
# It enable the following variables:
#   cppzmq                                the cppzmq library

# Find cppzmq installation
# Looks for cppzmq cmake config files installed by cppzmq's cmake installation.
find_package(ZeroMQ CONFIG REQUIRED HINTS "/opt/libzmq")
find_package(cppzmq CONFIG REQUIRED HINTS "/opt/cppzmq")
message(STATUS "Using cppzmq: ${cppzmq_VERSION}")

########################################################################################################################

robocin_cpp_project_setup(STANDARD 20)

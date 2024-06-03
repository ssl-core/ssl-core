########################################################################################################################

cmake_minimum_required(VERSION 3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../.cmake)

include(project)

project(protocols LANGUAGES C CXX)

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23 NO_SOURCE_DIRECTORY)

########################################################################################################################

function (get_protobufs prefix_path result)
  file(GLOB_RECURSE files "${prefix_path}/*.proto")
  set(${result} ${files} PARENT_SCOPE)
endfunction ()

get_protobufs("protocols" all_protos)

set(protos_only ${all_protos})
list(FILTER protos_only EXCLUDE REGEX "_service\.proto$")

set(services_only ${all_protos})
list(FILTER services_only INCLUDE REGEX "_service\.proto$")

########################################################################################################################

robocin_cpp_proto_library(
  NAME protobufs
  PROTOS ${protos_only}
)

robocin_cpp_nanopb_library(
  NAME nanopbs
  PROTOS ${protos_only}
)

robocin_cpp_grpc_library(
  NAME grpcs
  PROTOS ${services_only}
  DEPS protobufs
)

robocin_cpp_proto_library(
  NAME perception
  PROTOS ${perception_protos}
  DEPS common
)

########################################################################################################################

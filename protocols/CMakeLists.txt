########################################################################################################################

cmake_minimum_required(VERSION 3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../.cmake)

include(project)

project(protocols LANGUAGES CXX)

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23 NO_SOURCE_DIRECTORY)

########################################################################################################################

function(get_protobufs prefix_path result)
  file(GLOB_RECURSE files "${prefix_path}/*.proto")
  set(${result} ${files} PARENT_SCOPE)
endfunction()

get_protobufs("protocols/third_party" third_party_protos)

robocin_cpp_proto_library(
  NAME third_party
  PROTOS ${third_party_protos}
)

########################################################################################################################

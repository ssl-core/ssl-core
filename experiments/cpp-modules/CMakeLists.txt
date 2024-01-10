########################################################################################################################

cmake_minimum_required(VERSION 3.28)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../.cmake)

include(project)

project(external_target LANGUAGES CXX)

########################################################################################################################

find_package(common CONFIG REQUIRED)
message(STATUS "Using common: '${common_DIR}'")

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23 SOURCE_DIRECTORY experimental)

########################################################################################################################

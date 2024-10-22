########################################################################################################################

cmake_minimum_required(VERSION 3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../.cmake)

include(project)

project(communication LANGUAGES CXX)

########################################################################################################################

find_package(common CONFIG REQUIRED)
message(STATUS "Using common: '${common_DIR}'")

find_package(protocols CONFIG REQUIRED)
message(STATUS "Using protocols: '${protocols_DIR}'")

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23)

########################################################################################################################

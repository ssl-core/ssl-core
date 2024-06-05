########################################################################################################################

cmake_minimum_required(VERSION 3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../.cmake)

include(project)

########################################################################################################################

robocin_mbed_setup(TARGET NUCLEO_F767ZI TOOLCHAIN GCC_ARM)
project(nucleo-f767zi LANGUAGES CXX)

########################################################################################################################

find_package(common CONFIG REQUIRED)
message(STATUS "Using common: '${common_DIR}'")

# TODO(#121): add robots' communication protocols
# find_package(protocols CONFIG REQUIRED)
# message(STATUS "Using protocols: '${protocols_DIR}'")

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23 NO_SOURCE_DIRECTORY)

########################################################################################################################

add_subdirectory(firmware)

########################################################################################################################

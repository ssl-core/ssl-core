########################################################################################################################

cmake_minimum_required(VERSION 3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../.cmake)

include(project)

project(mongocxx-experiment LANGUAGES CXX)

########################################################################################################################

find_package(bson-1.0 CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using bson: '${bson-1.0_DIR}'")

find_package(mongoc-1.0 CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using mongoc: '${mongoc-1.0_DIR}'")

find_package(bsoncxx CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using bsonxx: '${bsoncxx_DIR}'")

find_package(mongocxx CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using mongocxx: '${mongocxx_DIR}'")

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23 SOURCE_DIRECTORY service)

########################################################################################################################

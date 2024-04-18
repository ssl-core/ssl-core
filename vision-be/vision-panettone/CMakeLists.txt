########################################################################################################################

cmake_minimum_required(VERSION 3.29)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../.cmake)

include(project)

project(vision LANGUAGES CXX)

########################################################################################################################

find_package(common CONFIG REQUIRED)
message(STATUS "Using common: '${common_DIR}'")

find_package(protocols CONFIG REQUIRED)
message(STATUS "Using protocols: '${protocols_DIR}'")

########################################################################################################################

find_package(bson-1.0 CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using bson: '${bson-1.0_DIR}'")

find_package(mongoc-1.0 CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using mongoc: '${mongoc-1.0_DIR}'")

find_package(bsoncxx CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using bsonxx: '${bsoncxx_DIR}'")

find_package(mongocxx CONFIG REQUIRED HINTS "/usr/local/mongocxx" "/opt/mongocxx")
message(STATUS "Using mongocxx: '${mongocxx_DIR}'")

set(mongocxx_deps mongo::mongoc_shared mongo::bson_shared mongo::mongocxx_shared mongo::bsoncxx_shared)

########################################################################################################################

robocin_cpp_project_setup(STANDARD 23)

add_subdirectory(microservice)

########################################################################################################################

// FrameRepositoryMongoDB.cpp
#include "vision/data/frame/frame_repository_mongo_db/frame_repository_mongo_db.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/json.hpp>
#include <cassert>
#include <iostream>
#include <mongocxx/collection.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

#include <thread>  // Include this at the top of your file

FrameRepositoryMongoDB::FrameRepositoryMongoDB() :
    instance_{},
    client_{mongocxx::uri{"mongodb://mongodb:27017/"}},
    db_{client_["frames_database"]},
    collection_{db_["frames_collection"]} {
  
  std::cout << __FUNCTION__ << " ENTRY" << "\n";
  
  // Retry connection to MongoDB
  const int kMaxRetries = 3;
  int retryCount = 0;
  while (retryCount < kMaxRetries) {
    try {
      std::cout << "Trying to connect to MongoDB" << "\n";
      
      // clearing the collection before initializing tests
      db_.drop();
      
      // Ping the database.
      const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
      db_.run_command(ping_cmd.view());
      std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << std::endl;  
      
      std::cout << "Connected to MongoDB" << "\n";

      break;
    } catch (const std::exception& e) {
      std::cout << "Error while connecting to MongoDB: " << e.what() << "\n";
      std::cout << "Retrying in " << kMaxRetries << " seconds...\n";
      std::this_thread::sleep_for(std::chrono::seconds(5));
      retryCount++;
    }
  }

  if (retryCount == kMaxRetries) {
    std::cout << "Failed to connect to MongoDB after 5 attempts. Exiting...\n";
    exit(1);
  }

  std::cout << __FUNCTION__ << " EXIT" << "\n";
}

void FrameRepositoryMongoDB::save(const Frame& frame) {
  std::cout << __FUNCTION__ << " ENTRY" << "\n";
  auto frame_id = frame.getId();
  try {
    auto insert_one_result = collection_.insert_one(make_document(kvp("_id", frame.getId())));
    assert(insert_one_result);
    std::cout << "Inserted frame with id: " << frame_id << "\n";
  } catch (const std::exception& e) {
    std::cerr << "Error saving frame: " << e.what() << "\n";
  }
}

std::optional<Frame> FrameRepositoryMongoDB::find(const std::string& frame_id) {
  std::cout << __FUNCTION__ << " ENTRY" << "\n";
  try {
    auto frame = collection_.find_one(make_document(kvp("_id", std::stoi(frame_id))));
    if (!frame) {
      std::cerr << "Frame not found" << "\n";
      return std::nullopt;
    }
    std::string frame_data = bsoncxx::to_json(frame->view());
    std::cout << "frame_data: " << frame_data << "\n";

    return Frame::fromJson(frame_data);
  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return std::nullopt;
  }
}

std::optional<Frame> FrameRepositoryMongoDB::update(const std::string& frame_id,
                                                    bsoncxx::document::value& update) {
  // try {
  //   auto filter = make_document(kvp("_id", frame_id));
  //   auto updated_frame = collection_.update_one(
  //       filter.view(),
  //       update.view());
  //   assert(updated_frame);

  //   std::string frame_data = bsoncxx::to_json(updated_frame->view());
  //   return Frame::fromJson(frame_data);
  // } catch (const std::exception& e) {
  //   std::cerr << "Error : " << e.what() << '\n';
  //   return std::nullopt;
  // }
  return std::nullopt;
}

void FrameRepositoryMongoDB::remove(const std::string& frame_id) {
  // try {
  //   auto deleted_frame = collection_.delete_one(make_document(kvp("_id", frame_id)));
  //   assert(deleted_frame);

  // } catch (const std::exception& e) {
  //   std::cerr << "Error : " << e.what() << '\n';
  // }
}

// FrameRepositoryMongoDB.cpp
#include "FrameRepositoryMongoDB.h"

#include <bsoncxx/json.hpp>
#include <iostream>

FrameRepositoryMongoDB::FrameRepositoryMongoDB() :
    client_{mongocxx::uri{"mongodb://mongodb:27017/"}},
    db_{client_["frames_database"]},
    collection_{db_["frames_collection"]} {}

void FrameRepositoryMongoDB::save(const Frame& frame) {
  int frame_id = frame.getId();
  try {

    std::string frameData = bsoncxx::document::value frameDoc = bsoncxx::from_json(frameData);
    bsoncxx::document::view frameView = frameDoc.view();

    // MongoDB does not allow using `_id` directly for queries, so we use a separate field 'mock'
    // here.
    bsoncxx::document::value frameDict
        = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("mock", frameId),
                                                 bsoncxx::builder::basic::kvp("data", frameView));

    collection_.insert_one(frameDict.view());
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

Frame FrameRepositoryMongoDB::find(const std::string& frameId) {
  try {
    bsoncxx::stdx::optional<bsoncxx::document::value> result = collection_.find_one(
        bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("mock", frameId)));

    if (result) {
      bsoncxx::document::view frameDict = result->view();
      std::string frameData = bsoncxx::to_json(frameDict["data"].get_value());
      return Frame::fromJson(frameData);
    } else {
      std::cerr << "Frame not found." << std::endl;
      // Handle the case when the frame is not found
      return Frame(); // Return a default-constructed Frame as a placeholder
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    // Handle the exception
    return Frame(); // Return a default-constructed Frame as a placeholder
  }
}

// FrameRepositoryMongoDB.cpp
#include "FrameRepositoryMongoDB.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>
#include <mongocxx/instance.hpp>

// Redefine assert after including headers. Release builds may undefine the assert macro and result
// in -Wunused-variable warnings.
#if defined(NDEBUG) || !defined(assert)
#undef assert
#define assert(stmt)                                                                               \
  do {                                                                                             \
    if (!(stmt)) {                                                                                 \
      std::cerr << "Assert on line " << __LINE__ << " failed: " << #stmt << "\n";                  \
      abort();                                                                                     \
    }                                                                                              \
  } while (0)
#endif

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

FrameRepositoryMongoDB::FrameRepositoryMongoDB() :
    client_{mongocxx::uri{"mongodb://mongodb:27017/"}},
    db_{client_["frames_database"]},
    collection_{db_["frames_collection"]} {}

void FrameRepositoryMongoDB::save(Frame& frame) {
  auto frame_id = frame.getId();
  try {
    auto doc_value = make_document(kvp("id", frame.getId()),
                                   kvp("balls", frame.getBalls()),
                                   kvp("robots", frame.getRobots()),
                                   kvp("field", frame.getField()));
    auto insert_one_result = collection_.insert_one(doc_value);
    assert(insert_one_result);
  } catch (const std::exception& e) {
    std::cerr << "Error saving frame: " << e.what() << "\n";
  }
}

Frame FrameRepositoryMongoDB::find(const std::string& frame_id) const {
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

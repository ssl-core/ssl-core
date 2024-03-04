#include "vision/db/frame_repository_mongodb.h"

#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <google/protobuf/util/json_util.h>

namespace vision {
namespace {

using bsoncxx::to_json;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using protocols::vision::Frame;

using MongoDbDocument = bsoncxx::builder::stream::document;
using MongoDbArray = bsoncxx::builder::stream::array;

using google::protobuf::util::JsonStringToMessage; // TODO($ISSUE_N): Remove if unused.

MongoDbDocument toMongoDbDocument(const Frame& frame) {
  MongoDbDocument document{};

  document << "_id" << static_cast<int64_t>(frame.properties().serial_id());

  MongoDbArray robots_doc{};
  for (const auto& robot : frame.robots()) {
    MongoDbDocument robot_doc{};
    robot_doc << "id" << robot.robot_id().number();
    robot_doc << "color" << robot.robot_id().color();

    // ...
  }

  // TODO($ISSUE_N): Fill remaining fields.

  return document;
}

} // namespace

FrameRepositoryMongoDb::FrameRepositoryMongoDb(const MongoDbRepositoryBuildArgs& args) :
    IMongoDbRepository(args) {}

void FrameRepositoryMongoDb::save(const Frame& frame) {
  auto frame_id = static_cast<int64_t>(frame.properties().serial_id());

  try {
    if (auto save_result = collection_.insert_one(toMongoDbDocument(frame).view())) {
      std::cout << "inserted frame with id: " << frame_id << ".\n";
    }
  } catch (const std::exception& e) {
    std::cerr << "error saving frame: " << e.what() << ".\n";
  }
}

void FrameRepositoryMongoDb::remove(const int64_t& key) {
  try {
    if (auto remove_result = collection_.delete_one(make_document(kvp("_id", key)))) {
      std::cout << "removed frame with id: " << key << ".\n";
    }
  } catch (const std::exception& e) {
    std::cerr << "error removing frame: " << e.what() << ".\n";
  }
}

std::optional<Frame> FrameRepositoryMongoDb::find(const int64_t& key) {
  try {
    if (auto find_result = collection_.find_one(make_document(kvp("_id", key)))) {
      std::string frame_json = to_json(find_result->view());

      // TODO($ISSUE_N): Parse frame_json to Frame (remove if unused).
      if (Frame frame; JsonStringToMessage(frame_json, &frame).ok()) {
        return frame;
      }

      // TODO($ISSUE_N): Parse from bytes read. (remove if unused).
      // if (Frame frame; frame.ParseFromString(...)) {
      //   return frame;
      // }

      std::cerr << "error parsing frame.\n";
      return std::nullopt;
    }

    std::cerr << "frame not found.\n";
    return std::nullopt;
  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return std::nullopt;
  }
}

} // namespace vision

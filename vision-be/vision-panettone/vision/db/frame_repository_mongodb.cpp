#include "vision/db/frame_repository_mongodb.h"

#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <google/protobuf/util/json_util.h>
#include <iostream>
#include <mutex>

namespace vision {
namespace {

using bsoncxx::to_json;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using protocols::vision::Ball;
using protocols::vision::Field;
using protocols::vision::Frame;
using protocols::vision::Robot;

using MongoDbDocumentView = bsoncxx::document::view;
using MongoDbDocument = bsoncxx::builder::stream::document;
using MongoDbArray = bsoncxx::builder::stream::array;

MongoDbDocument toMongoDbDocument(const Frame& frame) {
  MongoDbDocument document{};

  // document << "_id" << static_cast<int64_t>(frame.properties().serial_id());

  // std::cout << "serial_id: " << static_cast<int64_t>(frame.properties().serial_id()) << std::endl;

  MongoDbArray balls_array{};
  for (const auto& ball : frame.balls()) {
    MongoDbDocument ball_doc{};
    ball_doc << "confidence" << ball.confidence();
    ball_doc << "position" << bsoncxx::builder::stream::open_array << ball.position().x()
             << ball.position().y() << ball.position().z() << bsoncxx::builder::stream::close_array;
    ball_doc << "velocity" << bsoncxx::builder::stream::open_array << ball.velocity().x()
             << ball.velocity().y() << ball.position().z() << bsoncxx::builder::stream::close_array;
    balls_array << ball_doc;
  }

  MongoDbArray robots_array{};
  for (const auto& robot : frame.robots()) {
    MongoDbDocument robot_doc{};

    robot_doc << "confidence" << robot.confidence();

    robot_doc << "id" << robot.robot_id().number();
    
    robot_doc << "color" << robot.robot_id().color();
    
    robot_doc << "position" << bsoncxx::builder::stream::open_array << robot.position().x()
              << robot.position().y() << bsoncxx::builder::stream::close_array;
    
    robot_doc << "angle" << robot.angle();
    
    robot_doc << "velocity" << bsoncxx::builder::stream::open_array << robot.velocity().x()
              << robot.velocity().y() << bsoncxx::builder::stream::close_array;
    
    robot_doc << "angular_velocity" << robot.angular_velocity();

    robots_array << robot_doc;
  }
  document << "robots" << robots_array;

  MongoDbDocument field_doc{};
  const auto& field = frame.field();
  field_doc << "serial_id" << static_cast<int64_t>(field.serial_id());
  field_doc << "length" << field.length();
  field_doc << "width" << field.width();
  field_doc << "goal_depth" << field.goal_depth();
  field_doc << "goal_width" << field.goal_width();
  field_doc << "penalty_area_depth" << field.penalty_area_depth();
  field_doc << "penalty_area_width" << field.penalty_area_width();
  field_doc << "boundary_width" << field.boundary_width();
  field_doc << "goal_center_to_penalty_mark" << field.goal_center_to_penalty_mark();
  document << "field" << field_doc;

  return document;
}

Frame fromMongoDbDocument(MongoDbDocumentView doc) {
  Frame frame{};

  frame.mutable_properties()->set_serial_id(doc["_id"].get_int64());

  Field& field = *frame.mutable_field();
  field.set_serial_id(doc["field"]["serial_id"].get_int64());
  field.set_length(doc["field"]["length"].get_double());
  field.set_width(doc["field"]["width"].get_double());
  field.set_goal_depth(doc["field"]["goal_depth"].get_double());
  field.set_goal_width(doc["field"]["goal_width"].get_double());
  field.set_penalty_area_depth(doc["field"]["penalty_area_depth"].get_double());
  field.set_penalty_area_width(doc["field"]["penalty_area_width"].get_double());
  field.set_boundary_width(doc["field"]["boundary_width"].get_double());
  field.set_goal_center_to_penalty_mark(doc["field"]["goal_center_to_penalty_mark"].get_double());

  return frame;
}
} // namespace

FrameRepositoryMongoDb::FrameRepositoryMongoDb(const MongoDbRepositoryBuildArgs& args) :
    IMongoDbRepository(args) {}

void FrameRepositoryMongoDb::save(const Frame& frame) {
  std::cout << "Saving Frame with ID: " << static_cast<int64_t>(frame.properties().serial_id())
            << "." << std::endl;
  try {
    auto client = pool_.acquire();
    auto collection = (*client)[db_][collection_];

    MongoDbDocument document = toMongoDbDocument(frame);

    if (auto save_result = collection.insert_one(document.view())) {
      std::cout << "Inserted frame with id: "
                << static_cast<int64_t>(frame.properties().serial_id()) << "." << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error saving frame: " << e.what() << "." << std::endl;
  }
  std::cout << "Saved Frame with id: " << static_cast<int64_t>(frame.properties().serial_id())
            << "." << std::endl;
}

void FrameRepositoryMongoDb::saveMany(const std::vector<Frame>& frames) {
  std::cout << "Saving " << frames.size() << " frames." << std::endl;
  try {
    auto client = pool_.acquire();
    auto collection = (*client)[db_][collection_];

    std::vector<MongoDbDocument> documents;
    for(auto& frame: frames) {
      // std::cout << frame.DebugString() << std::endl;
      documents.push_back(toMongoDbDocument(frame));
    }

    std::cout << "Mongo document created for " << frames.size() << " frames." << std::endl;

    if (auto save_result = collection.insert_many(documents)) {
      std::cout << "Inserted frames." << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error saving frames: " << e.what() << "." << std::endl;
  }
  std::cout << "Saved frames." << std::endl;
}

void FrameRepositoryMongoDb::remove(const int64_t& key) {
  try {
    auto client = pool_.acquire();
    auto collection = (*client)[db_][collection_];
    if (auto remove_result = collection.delete_one(make_document(kvp("_id", key)))) {
      std::cout << "Removed frame with id: " << key << "." << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error removing frame: " << e.what() << "." << std::endl;
  }
}

std::optional<Frame> FrameRepositoryMongoDb::find(const int64_t& key) {
  try {
    auto client = pool_.acquire();
    auto collection = (*client)[db_][collection_];

    if (auto find_result = collection.find_one(make_document(kvp("_id", key)))) {

      std::string result = bsoncxx::to_json(*find_result);
      std::cout << "Result: " << result << std::endl;;

      return fromMongoDbDocument(find_result->view());
    }

    std::cerr << "Frame not found." << std::endl;
    return std::nullopt;
  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << std::endl;
    return std::nullopt;
  }
}

std::vector<Frame> FrameRepositoryMongoDb::findRange(const int64_t& key_lower_bound,
                                                     const int64_t& key_upper_bound) {
  std::cout << "Finding Frames in range [" << key_lower_bound << ", " << key_upper_bound
            << "]." << std::endl;
  try {
    auto client = pool_.acquire();
    auto collection = (*client)[db_][collection_];
    auto find_result = collection.find(make_document(
        kvp("_id", make_document(kvp("$gte", key_lower_bound), kvp("$lte", key_upper_bound)))));

    std::vector<Frame> frames;
    for (auto doc : find_result) {
      std::cout << "Key: " << doc["_id"].get_int64() << std::endl;;
      frames.push_back(fromMongoDbDocument(doc));
    }

    return frames;
  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << std::endl;
    return {};
  }
}

} // namespace vision

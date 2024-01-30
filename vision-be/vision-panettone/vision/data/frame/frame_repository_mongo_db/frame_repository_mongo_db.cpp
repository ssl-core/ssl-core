// FrameRepositoryMongoDB.cpp
#include "vision/data/frame/frame_repository_mongo_db/frame_repository_mongo_db.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/json.hpp>
#include <cassert>
#include <iostream>
#include <mongocxx/collection.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

FrameRepositoryMongoDB::FrameRepositoryMongoDB() :
    client_{mongocxx::uri{"mongodb://mongodb:27017/"}},
    db_{client_["frames_database"]},
    collection_{db_["frames_collection"]} {}

void FrameRepositoryMongoDB::save(const Frame& frame) {
  auto frame_id = frame.getId();
  try {
    auto document = make_document(kvp("_id", frame.getId()),
                                  kvp("balls", frame.getBalls()),
                                  kvp("robots", frame.getRobots()),
                                  kvp("field", frame.getField()));
    auto insert_one_result = collection_.insert_one(document.view());
    assert(insert_one_result);
  } catch (const std::exception& e) {
    std::cerr << "Error saving frame: " << e.what() << "\n";
  }
}

std::optional<Frame> FrameRepositoryMongoDB::find(const std::string& frame_id) {
  try {
    auto frame = collection_.find_one(make_document(kvp("_id", frame_id)));
    assert(frame);

    std::string frame_data = bsoncxx::to_json(frame->view());
    return Frame::fromJson(frame_data);
  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return std::nullopt;
  }
}

std::optional<Frame> FrameRepositoryMongoDB::update(const std::string& frame_id,
                                                    bsoncxx::document::value& update) {
  try {
    auto filter = make_document(kvp("_id", frame_id));
    auto updated_frame = collection_.find_one_and_update(
        filter.view(),
        update.view()); // TODO(aqb): verify if find_one_and_update is the right method
    assert(updated_frame);

    std::string frame_data = bsoncxx::to_json(updated_frame->view());
    return Frame::fromJson(frame_data);
  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
    return std::nullopt;
  }
}

void FrameRepositoryMongoDB::remove(const std::string& frame_id) {
  try {
    auto deleted_frame = collection_.delete_one(make_document(kvp("_id", frame_id)));
    assert(deleted_frame);

  } catch (const std::exception& e) {
    std::cerr << "Error : " << e.what() << '\n';
  }
}

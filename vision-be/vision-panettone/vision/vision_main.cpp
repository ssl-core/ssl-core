#include "protocols/vision/frame.pb.h"
#include "vision/db/irepository_factory.h"
#include "vision/db/repository_factory_mapping.h"

#include <iostream>
#include <vector>

using protocols::vision::Field;
using protocols::vision::Frame;
using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

struct TestInputs {
  int64_t record_id;
  std::string operation;
};

int main() {
  std::cout << "Creating factory and frame repository"
            << "\n";
  const auto kFactory = RepositoryFactoryMapping{}[RepositoryType::MongoDb];

  std::unique_ptr<IFrameRepository> frame_repository = kFactory->createFrameRepository();
  std::future<bool> connection_status = frame_repository->connect();

  if (connection_status.wait(); connection_status.get()) {
    std::cout << "Connected to the database."
              << "\n";
  } else {
    std::cout << "Failed to connect to the database."
              << "\n";
    return 1;
  }

  std::cout << "Creating test inputs"
            << "\n";
  std::vector<TestInputs> test_inputs = {{0, "save"},
                                         {0, "save"},
                                         {1, "find"},
                                         {2, "find"},
                                         {0, "save"},
                                         {3, "find"},
                                         {3, "remove"},
                                         {3, "find"},
                                         {2, "find_range"}};

  int serial_id = 0;
  for (const auto& [record_id, operation] : test_inputs) {
    if (operation == "find") {
      std::cout << "fetching (key=" << record_id << ")...\n";
      if (auto result = frame_repository->find(record_id)) {
        std::cout << "retrieved frame: " << result->DebugString() << ".\n";
      } else {
        std::cout << "frame not found.\n";
      }

    } else if (operation == "find_range") {
      std::cout << "fetching range 1 >= key <= " << record_id << ")...\n";
      if (auto result = frame_repository->findRange(1, record_id)) {
        std::cout << "retrieved frames \n";
        for (const auto& frame : *result) {
          std::cout << frame.DebugString() << "\n";
        }
      } else {
        std::cout << "frames not found.\n";
      }
    } else if (operation == "remove") {
      std::cout << "removing (key=" << record_id << ")...\n";
      frame_repository->remove(record_id);

    } else if (operation == "save") {
      std::cout << "saving...\n";

      Frame frame;
      // Set Frame id
      frame.mutable_properties()->set_serial_id(++serial_id);

      // Set field properties
      Field& field = *frame.mutable_field();
      field.set_length(9000);
      field.set_width(6000);
      field.set_goal_depth(180);
      field.set_goal_width(1000);
      field.set_penalty_area_depth(1000);
      field.set_penalty_area_width(2000);
      field.set_boundary_width(300);
      field.set_goal_center_to_penalty_mark(6000);

      frame_repository->save(frame);

      std::cout << "saved frame: " << frame.DebugString() << ".\n";
    } else {
      std::cout << "invalid operation: " << operation << " \n";
    }
  }

  return 0;
}

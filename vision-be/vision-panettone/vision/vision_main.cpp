#include "vision/repository_factory/repository_factory_mapping/repository_factory_mapping.h"
#include "vision/src/thread_pool/thread_pool.h"

#include <iostream>
#include <vector>

// Hypothetical database function
std::string fetchDataFromDatabase(int record_id) {
  // Simulate some time-consuming database operation
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return "Data for record " + std::to_string(record_id);
}

int fetchDataFromDatabase(const std::string& record_id, IFrameRepository* frame_repository) {
  std::optional<Frame> frame;
  frame = frame_repository->find(record_id);
  if (frame.has_value()) {
    return frame->getId();
  }
  return -1;
}

int saveDataToDatabase(const std::string& record_id, IFrameRepository* frame_repository) {
  int id = std::stoi(record_id);
  frame_repository->save(Frame(id, {}, {}, kDefaultField));
  return id;
}

int main() {
  const size_t num_threads = 4;
  ThreadPool pool(num_threads);

  auto factory_mapping = std::make_shared<RepositoryFactoryMapping>();
  auto factory = factory_mapping->getFactory(RepositoryType::MONGODB);
  auto* frame_repository = factory->createFrameRepository().release();

  std::vector<std::future<int>> results;

  while (true) {
    std::cout << "Enter operation (fetch, save or quit): ";
    std::string operation;
    std::cin >> operation;

    if (operation == "quit") {
      break;
    }

    std::cout << "Enter record id: ";
    std::string record_id;
    std::cin >> record_id;

    auto save = [id = record_id, frame_repository] -> int {
      return saveDataToDatabase(id, frame_repository);
    };

    auto fetch = [id = record_id, frame_repository] -> int {
      return fetchDataFromDatabase(id, frame_repository);
    };

    if (operation == "fetch") {
      results.push_back(pool.enqueue(fetch));
    } else if (operation == "save") {
      results.push_back(pool.enqueue(save));
    } else {
      std::cout << "Invalid operation\n";
    }

    std::cout << "NEXT\n";
  }

  return 0;
}

#include "vision/repository_factory/repository_factory_mapping/repository_factory_mapping.h"

#include <iostream>
#include <vector>

struct TestInputs {
  std::string record_id;
  std::string operation;
};

int main() {
  std::cout << __FUNCTION__ << " ENTRY" << "\n";
  const size_t num_threads = 4;
  
  std::cout << "Creating factory and frame repository" << "\n";
  auto factory_mapping = std::make_shared<RepositoryFactoryMapping>();
  auto factory = factory_mapping->getFactory(RepositoryType::MONGODB);
  auto* frame_repository = factory->createFrameRepository().release();

  std::cout << "Creating test inputs" << "\n";

  // test input for the program
  std::vector<TestInputs> test_inputs = { {"1", "save"}, {"2", "save"}, {"1", "fetch"}, {"2", "fetch"} };

  auto it_tests = test_inputs.begin();
  while (it_tests != test_inputs.end()) {
    std::string operation = it_tests->operation;
    std::cout << "Operation: " << operation << "\n";

    if (operation == "quit") {
      break;
    }

    std::string record_id = it_tests->record_id;
    std::cout << "Record id is: " << record_id << "\n";

    if (operation == "fetch") {
      std::cout << "fetch\n";
      auto ret = frame_repository->find(record_id);
      std::cout << "Retrieved frame with id: " << ret->getId() << "\n";
    } else if (operation == "save") {
      std::cout << "save\n";
      frame_repository->save(Frame(std::stoi(record_id), {}, {}, kDefaultField));
    } else {
      std::cout << "Invalid operation\n";
    }

    it_tests++;
  }

  std::cout << __FUNCTION__ << " EXIT" << "\n";
  return 0;
}

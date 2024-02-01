#include "vision/src/thread_pool/thread_pool.h"

#include <iostream>
#include <vector>

// Hypothetical database function
std::string fetchDataFromDatabase(int record_id) {
  // Simulate some time-consuming database operation
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return "Data for record " + std::to_string(record_id);
}

int main() {
  const size_t num_threads = 4;
  ThreadPool pool(num_threads);

  std::vector<std::future<std::string>> results;

  results.reserve(8);
  for (int i = 0; i < 8; ++i) {
    results.push_back(pool.enqueue(fetchDataFromDatabase, i));
  }

  // Wait for all tasks to complete and retrieve results
  for (auto& result : results) {
    std::cout << "Result: " << result.get() << '\n';
  }

  return 0;
}

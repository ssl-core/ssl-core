# Experiment: Thread Pool

## Table of Contents
- [Introduction](#Introduction)
- [Usage](#Usage)
- [Example](#Example)
- [Testing](#Testing)

## Introduction

The `ThreadPool` class provides a mechanism for managing a pool of worker threads that can execute arbitrary tasks concurrently. This allows for efficient utilization of available CPU cores and improved performance for applications that require parallel execution of tasks.

## Usage

To use the `ThreadPool` class, follow these steps:
1. Create a `ThreadPool` object with the desired number of threads.
2. Enqueue tasks using the `enqueue` method, providing the task function and any required arguments.
3. Wait for the tasks to complete and retrieve their results if necessary.
4. When finished, stop the thread pool by calling the `stop` method, so the thread pool can no longer be used to enqueue tasks.
5. A `ThreadPool` can be restarted by calling the `restart` method, allowing the thread pool to be used for enqueuing tasks again after it has been stopped.

## Example

```cpp
#include "robocin/utility/thread_pool/thread_pool.h"

#include <iostream>
#include <vector>

// Example task function
std::string fetchDataFromDatabase(int record_id) {
  // Simulate some time-consuming operation
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return "Data for record " + std::to_string(record_id);
}

int main() {
  const size_t num_threads = 4;
  ThreadPool pool(num_threads);

  std::vector<std::future<std::string>> results;

  results.reserve(8);
  for (int i = 0; i < 8; ++i) {
    // Enqueue tasks to fetch data from a database
    results.push_back(pool.enqueue(fetchDataFromDatabase, i));
  }

  // Wait for all tasks to complete and retrieve results
  for (auto& result : results) {
    std::cout << "Result: " << result.get() << '\n';
  }

  return 0;
}
```

## Testing

To test the behavior, build the container and run the `thread_pool_main` executable located in bin folder. 

TODO: This testing code should be updated to a valid integration with a database after the PR [MongoDB Experiment with C++ #26](https://github.com/robocin/ssl-core/pull/26) is being merged.
# concurrency

A collection of concurrency functions and classes.

## Table of Contents

- [concurrency](#concurrency)
  - [Table of Contents](#table-of-contents)
  - [`thread_pool`](#thread_pool)

## [`thread_pool`](thread_pool.h)

The [thread_pool](thread_pool.h) header provides a mechanism for managing a pool of worker threads that can execute arbitrary tasks concurrently. This allows for efficient utilization of available CPU cores and improved performance for applications that require parallel execution of tasks.

The `ThreadPool` class consists of the following key components:
- `ThreadPool`: The constructor initializes the thread pool with a specified number of worker threads.
- `enqueue`: This method adds a new task to the task queue and returns a future representing the result of the task's execution.
- `stop`: This method stops the thread pool by setting the flag `stop_` to `true` and notifying all worker threads to exit their main loop.

## [`blocking_deque`](blocking_deque.h)

The [blocking_deque](blocking_deque.h) header provides a mechanism for managing a double-ended queue (deque) in a thread-safe manner. This allows for concurrent access to the deque, ensuring that resources are managed safely in a multi-threaded environment.

The `BlockingDeque` class consists of the following key components:
- `BlockingDeque`: The constructor initializes the deque.
- `put`: This method adds a resource to the deque.
- `take`:  This method removes the first resource from the deque and returns it.
- `takeAll`: This method removes all resources from the deque and returns them as a vector.

The `BlockingDeque` interface has three concrete implementations to provide a thread-safe deque management.  
- The `AtomicBlockingDeque` class provides a atomic-based implementation of the `BlockingDeque`.
- The `ConditionalVariableBlockingDeque` class provides a conditional-variable-based implementation of the `BlockingDeque`.
- The `MutexBlockingDeque` class provides a mutex-based implementation of the `BlockingDeque`.


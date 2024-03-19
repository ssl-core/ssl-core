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

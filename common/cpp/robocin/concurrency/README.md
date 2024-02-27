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
- `~ThreadPool()`: The destructor ensures proper cleanup by stopping all worker threads and joining them.
- `enqueue`: This method adds a new task to the task queue and returns a future representing the result of the task's execution.
- `restart`: This method restart the thread pool by setting the flag `stop_` to `false` and initializing the worker threads, allowing the thread pool to be used for enqueuing tasks again after it has been stopped, i.e., if the thread pool is already running, calling this method has no effect.
- `stop`: This method stops the thread pool by setting the flag `stop_` to `true` and notifying all worker threads to exit their main loop.
- `workLoop`: This is a private method that represents the main loop executed by each worker thread. It waits for tasks to be added to the task queue and executes them.

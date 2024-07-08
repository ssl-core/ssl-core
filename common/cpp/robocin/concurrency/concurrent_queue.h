#ifndef ROBOCIN_CONCURRENCY_CONCURRENT_QUEUE_H
#define ROBOCIN_CONCURRENCY_CONCURRENT_QUEUE_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include <condition_variable>
#include <deque>
#include <mutex>
#include <vector>

namespace robocin {

/**
 * @brief A thread-safe double-ended queue (deque) for managing values.
 *
 * @tparam T The type of values to be stored in the concurrent queue.
 */
template <class T, class Container = std::deque<T>>
class IConcurrentQueue {
 public:
  using container_type = Container;
  using value_type = Container::value_type;
  using size_type = Container::size_type;

  IConcurrentQueue() = default;

  IConcurrentQueue(const IConcurrentQueue&) = delete;
  IConcurrentQueue& operator=(const IConcurrentQueue&) = delete;
  IConcurrentQueue(IConcurrentQueue&&) = default;
  IConcurrentQueue& operator=(IConcurrentQueue&&) = default;

  ~IConcurrentQueue() = default;

  /**
   * @brief Adds a value to the concurrent queue.
   *
   * @param value The value to be added to the concurrent queue.
   */
  virtual void push(const value_type& value) = 0;
  virtual void push(value_type&& value) = 0;

  /**
   * @brief Removes the first value from the concurrent queue and returns it.
   *
   * @return The first value that was in the concurrent queue.
   */
  virtual value_type dequeue() = 0;

  /**
   * @brief Removes all values from the concurrent queue and returns them as a vector.
   *
   * @return A vector containing all the values that were in the concurrent queue.
   */
  virtual std::vector<value_type> dequeue_all() = 0; // NOLINT(*naming*)

  // TODO(matheusvtna): add capacity methods
  // [[nodiscard]] virtual bool empty() const = 0;
  // [[nodiscard]] virtual size_type size() const = 0;
};

template <class T, class Container = std::deque<T>>
class AtomicConcurrentQueue : public IConcurrentQueue<T, Container> {
 public:
  using container_type = IConcurrentQueue<T, Container>::container_type;
  using value_type = IConcurrentQueue<T, Container>::value_type;
  using size_type = IConcurrentQueue<T, Container>::size_type;

  /**
   * @brief Adds a value to the concurrent queue in a thread-safe manner using an atomic approach.
   *
   * @param value The value to be added to the concurrent queue.
   */
  void push(const value_type& value) override {
    while (atomic_locker_.exchange(true, std::memory_order_acquire)) {
    }
    container_.push_back(value);
    atomic_locker_.store(false, std::memory_order_release);
  }
  void push(value_type&& value) override {
    while (atomic_locker_.exchange(true, std::memory_order_acquire)) {
    }
    container_.push_back(std::move(value));
    atomic_locker_.store(false, std::memory_order_release);
  }

  /**
   * @brief Removes the first value from the concurrent queue in a thread-safe manner using an
   * atomic approach and returns it.
   *
   * @return The first value that was in the concurrent queue.
   */
  value_type dequeue() override {
    while (atomic_locker_.exchange(true, std::memory_order_acquire)) {
    };
    value_type value = container_.front();
    container_.pop_front();
    atomic_locker_.store(false, std::memory_order_release);
    return std::move(value);
  }

  /**
   * @brief Removes all values from the concurrent queue in a thread-safe manner using an atomic
   * approach and returns them as a vector.
   *
   * @return A vector containing all the values that were in the concurrent queue.
   */
  std::vector<T> dequeue_all() override {
    while (atomic_locker_.exchange(true, std::memory_order_acquire)) {
    }
    std::vector<T> values(std::make_move_iterator(container_.begin()),
                          std::make_move_iterator(container_.end()));
    container_.clear();
    atomic_locker_.store(false, std::memory_order_release);
    return values;
  }

 private:
  container_type container_;
  std::atomic<bool> atomic_locker_;
};

template <class T, class Container = std::deque<T>>
class ConditionVariableConcurrentQueue : public IConcurrentQueue<T, Container> {
 public:
  using container_type = IConcurrentQueue<T, Container>::container_type;
  using value_type = IConcurrentQueue<T, Container>::value_type;
  using size_type = IConcurrentQueue<T, Container>::size_type;

  /**
   * @brief Adds a value to the concurrent queue in a thread-safe manner using a conditional
   * variable approach.
   *
   * @param value The value to be added to the concurrent queue.
   */
  void push(const value_type& value) override {
    std::unique_lock lock(mutex_);
    container_.push_back(value);
    cv_.notify_one();
  }
  void push(value_type&& value) override {
    std::unique_lock lock(mutex_);
    container_.push_back(std::move(value));
    cv_.notify_one();
  }

  /**
   * @brief Removes the first value from the concurrent queue in a thread-safe manner using a
   * conditional variable approach and returns it.
   *
   * @return The first value that was in the concurrent queue.
   */
  value_type dequeue() override {
    std::unique_lock lock(mutex_);
    cv_.wait(lock, [this]() { return !container_.empty(); });
    value_type value = container_.front();
    container_.pop_front();
    return value;
  }

  /**
   * @brief Removes all values from the concurrent queue in a thread-safe manner using a condition
   * variable approach and returns them as a vector.
   *
   * @return A vector containing all the values that were in the concurrent queue.
   */
  std::vector<value_type> dequeue_all() override {
    std::unique_lock lock(mutex_);
    cv_.wait(lock, [this]() { return !container_.empty(); });
    std::vector<T> values(std::make_move_iterator(container_.begin()),
                          std::make_move_iterator(container_.end()));
    container_.clear();
    return values;
  }

 private:
  container_type container_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

/**
 * @brief A mutex-based implementation of the IConcurrentQueue for thread-safety.
 *
 * @tparam value The type of values to be stored in the concurrent queue.
 */
template <class T, class Container = std::deque<T>>
class MutexConcurrentQueue : public IConcurrentQueue<T, Container> {
 public:
  using container_type = IConcurrentQueue<T, Container>::container_type;
  using value_type = IConcurrentQueue<T, Container>::value_type;
  using size_type = IConcurrentQueue<T, Container>::size_type;

  /**
   * @brief Adds a value to the concurrent queue in a thread-safe manner using a mutex approach.
   *
   * @param value The value to be added to the concurrent queue.
   */
  void push(const value_type& value) override {
    std::lock_guard lock(mutex_);
    container_.push_back(value);
  }
  void push(value_type&& value) override {
    std::lock_guard lock(mutex_);
    container_.push_back(value);
  }

  /**
   * @brief Removes the first value from the concurrent queue in a thread-safe manner using a mutex
   * approach and returns it.
   *
   * @return The first value that was in the concurrent queue.
   */
  value_type dequeue() override {
    std::lock_guard lock(mutex_);
    value_type value = container_.front();
    container_.pop_front();
    return value;
  }

  /**
   * @brief Removes all values from the concurrent queue in a thread-safe manner using a mutex
   * approach and returns them as a vector.
   *
   * @return A vector containing all the values that were in the concurrent queue.
   */
  std::vector<value_type> dequeue_all() override {
    std::lock_guard lock(mutex_);
    std::vector<T> values(std::make_move_iterator(container_.begin()),
                          std::make_move_iterator(container_.end()));
    container_.clear();
    return values;
  }

 private:
  container_type container_;
  std::mutex mutex_;
};

} // namespace robocin

#endif

#endif // ROBOCIN_CONCURRENCY_CONCURRENT_QUEUE_H

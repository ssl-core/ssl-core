#ifndef ROBOCIN_CONCURRENCY_BLOCKING_DEQUE_H
#define ROBOCIN_CONCURRENCY_BLOCKING_DEQUE_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include <condition_variable>
#include <deque>
#include <mutex>
#include <vector>

namespace robocin {

/**
 * @brief A thread-safe double-ended queue (deque) for managing resources.
 *
 * @tparam Resource The type of resources to be stored in the deque.
 */
template <class Resource>
class BlockingDeque {
 public:
  BlockingDeque() = default;
  ~BlockingDeque() = default;
  BlockingDeque(const BlockingDeque&) = delete;
  BlockingDeque& operator=(const BlockingDeque&) = delete;
  BlockingDeque(BlockingDeque&&) = default;
  BlockingDeque& operator=(BlockingDeque&&) = default;

  /**
   * @brief Adds a resource to the deque.
   *
   * @param resource The resource to be added to the deque.
   */
  virtual void put(const Resource& resource) = 0;

  /**
   * @brief Removes the first resource from the deque and returns it.
   *
   * @return The first resource that was in the deque.
   */
  virtual Resource take() = 0;

  /**
   * @brief Removes all resources from the deque and returns them as a vector.
   *
   * @return A vector containing all the resources that were in the deque.
   */
  virtual std::vector<Resource> takeAll() = 0;

 protected:
  std::deque<Resource> deque_; ///< The underlying double-ended queue to store resources. // NOLINT
};

template <class Resource>
class AtomicBlockingDeque : public BlockingDeque<Resource> {

  /**
   * @brief Adds a resource to the deque in a thread-safe manner using an atomic approach.
   *
   * @param resource The resource to be added to the deque.
   */
  void put(const Resource& resource) override {
    while (locked_.exchange(true, std::memory_order_acquire)) {
    };
    this->deque_.push_back(resource);
    locked_.store(false, std::memory_order_release);
  }

  /**
   * @brief Removes the first resource from the deque in a thread-safe manner using an atomic
   * approach and returns it.
   *
   * @return The first resource that was in the deque.
   */
  Resource take() override {
    while (locked_.exchange(true, std::memory_order_acquire)) {
    };
    auto resource = this->deque_.front();
    this->deque_.pop_front();
    locked_.store(false, std::memory_order_release);
    return resource;
  }

  /**
   * @brief Removes all resources from the deque in a thread-safe manner using an atomic approach
   * and returns them as a vector.
   *
   * @return A vector containing all the resources that were in the deque.
   */
  std::vector<Resource> takeAll() override {
    while (locked_.exchange(true, std::memory_order_acquire)) {
    };
    std::vector<Resource> resources(this->deque_.begin(), this->deque_.end());
    this->deque_.clear();
    locked_.store(false, std::memory_order_release);
    return resources;
  }

 private:
  std::atomic<bool> locked_;
};

template <class Resource>
class ConditionVariableBlockingDeque : public BlockingDeque<Resource> {
  /**
   * @brief Adds a resource to the deque in a thread-safe manner using a conditional variable
   * approach.
   *
   * @param resource The resource to be added to the deque.
   */
  void put(const Resource& resource) override {
    std::unique_lock<std::mutex> lock(mutex_);
    this->deque_.push_back(resource);
    cv_.notify_one();
  }

  /**
   * @brief Removes the first resource from the deque in a thread-safe manner using a conditional
   * variable approach and returns it.
   *
   * @return The first resource that was in the deque.
   */
  Resource take() override {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() { return !this->deque_.empty(); });
    auto resource = this->deque_.front();
    this->deque_.pop_front();
    return resource;
  }

  /**
   * @brief Removes all resources from the deque in a thread-safe manner using a condition variable
   * approach and returns them as a vector.
   *
   * @return A vector containing all the resources that were in the deque.
   */
  std::vector<Resource> takeAll() override {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() { return !this->deque_.empty(); });
    std::vector<Resource> resources(this->deque_.begin(), this->deque_.end());
    this->deque_.clear();
    return resources;
  }

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
};

/**
 * @brief A mutex-based implementation of the BlockingDeque for thread-safety.
 *
 * @tparam Resource The type of resources to be stored in the deque.
 */
template <class Resource>
class MutexBlockingDeque : public BlockingDeque<Resource> {
  /**
   * @brief Adds a resource to the deque in a thread-safe manner using a mutex approach.
   *
   * @param resource The resource to be added to the deque.
   */
  void put(const Resource& resource) override {
    std::lock_guard<std::mutex> lock(mutex_);
    this->deque_.push_back(resource);
  }

  /**
   * @brief Removes the first resource from the deque in a thread-safe manner using a mutex approach
   * and returns it.
   *
   * @return The first resource that was in the deque.
   */
  Resource take() override {
    std::lock_guard<std::mutex> lock(mutex_);
    auto resource = this->deque_.front();
    this->deque_.pop_front();
    return resource;
  }

  /**
   * @brief Removes all resources from the deque in a thread-safe manner using a mutex approach and
   * returns them as a vector.
   *
   * @return A vector containing all the resources that were in the deque.
   */
  std::vector<Resource> takeAll() override {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Resource> resources(this->deque_.begin(), this->deque_.end());
    this->deque_.clear();
    return resources;
  }

 private:
  std::mutex mutex_; ///< The mutex used to lock and unlock the deque.
};

} // namespace robocin

#endif

#endif // ROBOCIN_CONCURRENCY_BLOCKING_DEQUE_H

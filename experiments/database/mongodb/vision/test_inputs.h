#ifndef VISION_FRAME_REPOSITORY_TEST_TEST_INPUTS_H
#define VISION_FRAME_REPOSITORY_TEST_TEST_INPUTS_H

#include "protocols/vision/frame.pb.h"

#include <string>
#include <utility>

namespace vision::frame_repository_test {

using protocols::vision::Frame;
class TestInput {
 public:
  TestInput() = default;
  virtual ~TestInput() = default;
  TestInput(const TestInput&) = default;
  TestInput& operator=(const TestInput&) = default;
  TestInput(TestInput&&) = default;
  TestInput& operator=(TestInput&&) = default;
  [[nodiscard]] virtual const std::string& getCommandType() const = 0;
};

class SaveInput : public TestInput {
 public:
  explicit SaveInput(Frame frame) : frame_(std::move(frame)) {}

  const std::string& getCommandType() const override { return kCommandType_; }
  Frame frame() const { return frame_; }

 private:
  std::string kCommandType_ = "save";
  Frame frame_;
};

class FindInput : public TestInput {
 public:
  explicit FindInput(int id) : id_(id) {}

  [[nodiscard]] const std::string& getCommandType() const override { return kCommandType_; }
  [[nodiscard]] int id() const { return id_; }

 private:
  std::string kCommandType_ = "find";
  int id_;
};

class FindRangeInput : public TestInput {
 public:
  explicit FindRangeInput(int64_t key_lower_bound, int64_t key_upper_bound) :
      key_lower_bound_(key_lower_bound),
      key_upper_bound_(key_upper_bound) {}

  [[nodiscard]] const std::string& getCommandType() const override { return kCommandType_; }
  [[nodiscard]] int64_t keyLowerBound() const { return key_lower_bound_; }
  [[nodiscard]] int64_t keyUpperBound() const { return key_upper_bound_; }

 private:
  std::string kCommandType_ = "find_range";
  int64_t key_lower_bound_;
  int64_t key_upper_bound_;
};

class RemoveInput : public TestInput {
 public:
  explicit RemoveInput(int id) : id_(id) {}

  [[nodiscard]] const std::string& getCommandType() const override { return kCommandType_; }
  [[nodiscard]] int id() const { return id_; }

 private:
  std::string kCommandType_ = "remove";
  int id_;
};

} // namespace vision::frame_repository_test

#endif // VISION_FRAME_REPOSITORY_TEST_TEST_INPUTS_H

#ifndef VISION_FRAME_REPOSITORY_TEST_COMMANDS_H
#define VISION_FRAME_REPOSITORY_TEST_COMMANDS_H

#include "vision/db/iframe_repository.h"
#include "vision/test_inputs.h"

#include <iostream>
#include <memory>
#include <optional>

namespace vision::frame_repository_test {

using protocols::vision::Frame;

template <class ReturnType>
class Command {
 public:
  Command() = default;
  virtual ~Command() = default;
  Command(const Command&) = default;
  Command& operator=(const Command&) = default;
  Command(Command&&) = default;
  Command& operator=(Command&&) = default;
  virtual ReturnType execute(std::unique_ptr<IFrameRepository> repository, TestInput* input) = 0;
};

class SaveCommand : public Command<void> {
 public:
  SaveCommand();

  void execute(std::unique_ptr<IFrameRepository> repository, TestInput* input) override {
    auto* save_input = dynamic_cast<SaveInput*>(input);
    if (save_input != nullptr) {
      auto func = [&]() {
        Frame frame = save_input->frame();
        std::cout << "Saving Frame " << frame.DebugString() << std::endl;
        repository->save(frame);
        std::cout << "Saved Frame!" << std::endl;
      };
      func();
      return;
    }
    std::cout << "Invalid input for SaveCommand." << std::endl;
  }
};

class FindCommand : public Command<std::optional<Frame>> {
 public:
  FindCommand();

  std::optional<Frame> execute(std::unique_ptr<IFrameRepository> repository,
                               TestInput* input) override {
    auto* find_input = dynamic_cast<FindInput*>(input);
    if (find_input != nullptr) {
      int id = find_input->id();
      auto func = [&]() {
        std::cout << "Finding Frame with ID " << id << std::endl;
        if (auto frame = repository->find(id)) {
          std::cout << "Retrieved Frame " << frame->DebugString() << std::endl;
          return frame;
        }
        std::cout << "Couldn't find a Frame with ID " << id << std::endl;
        return std::optional<Frame>();
      };
      const std::optional<Frame> frame = func();
      return frame;
    }
    std::cout << "Invalid input for FindCommand." << std::endl;
    return {};
  }
};

class FindRangeCommand : public Command<std::vector<Frame>> {
 public:
  FindRangeCommand();

  std::vector<Frame> execute(std::unique_ptr<IFrameRepository> repository,
                             TestInput* input) override {
    auto* find_range_input = dynamic_cast<FindRangeInput*>(input);
    if (find_range_input != nullptr) {
      int64_t key_lower_bound = find_range_input->keyLowerBound();
      int64_t key_upper_bound = find_range_input->keyUpperBound();
      auto func = [&]() {
        std::cout << "Finding Frame where " << key_lower_bound << "=< key <= " << key_upper_bound
                  << std::endl;
        const std::vector<Frame> frames = repository->findRange(key_lower_bound, key_upper_bound);
        std::cout << "Retrieved Frames" << std::endl;
        for (const auto& frame : frames) {
          std::cout << frame.DebugString() << std::endl;
        }
        return frames;
      };
      const std::vector<Frame> frames = func();
      return frames;
    }
    std::cout << "Invalid input for FindRangeCommand." << std::endl;
    return {};
  }
};

class RemoveCommand : public Command<void> {
 public:
  void execute(std::unique_ptr<IFrameRepository> repository, TestInput* input) override {
    auto* remove_input = dynamic_cast<RemoveInput*>(input);
    if (remove_input != nullptr) {
      auto func = [&]() {
        int id = remove_input->id();
        std::cout << "Removing Frame with ID " << id << std::endl;
        repository->remove(id);
        std::cout << "Removed Frame with ID " << id << std::endl;
      };
      func();
      return;
    }
    std::cout << "Invalid input for RemoveCommand." << std::endl;
  }
};

} // namespace vision::frame_repository_test

#endif // VISION_FRAME_REPOSITORY_TEST_COMMANDS_H

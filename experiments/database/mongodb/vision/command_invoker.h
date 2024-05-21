#ifndef VISION_FRAME_REPOSITORY_TEST_COMMAND_INVOKER_H
#define VISION_FRAME_REPOSITORY_TEST_COMMAND_INVOKER_H

#include "vision/commands.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vision::frame_repository_test {
class CommandInvoker {
 public:
  void addCommand(const std::string& command_type, std::unique_ptr<Command<TestInput>> command) {
    commandMap_[command_type] = std::move(command);
  }

  void executeCommands(const std::unique_ptr<IFrameRepository> repository,
                       const std::vector<TestInput*>& inputs) {
    for (auto* const input : inputs) {
      auto it = commandMap_.find(input->getCommandType());
      if (it != commandMap_.end()) {
        it->second->execute(std::move(repository), input);
      } else {
        std::cout << "There is no supported command for type: " << input->getCommandType()
                  << std::endl;
      }
    }
  }

 private:
  std::unordered_map<std::string, std::unique_ptr<Command<TestInput>>> commandMap_;
};
} // namespace vision::frame_repository_test

#endif // VISION_FRAME_REPOSITORY_TEST_COMMAND_INVOKER_H

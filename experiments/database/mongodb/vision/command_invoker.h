#ifndef VISION_FRAME_REPOSITORY_TEST_COMMAND_INVOKER_H
#define VISION_FRAME_REPOSITORY_TEST_COMMAND_INVOKER_H

#include "vision/commands.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vision {
    
    namespace frame_repository_test {
        class CommandInvoker {
        public:
            void addCommand(const string& commandType, Command<TestInput>* command) {
                commandMap[commandType] = command;
            }

            void executeCommands(const IFrameRepository* repository, const std::vector<TestInput*>& inputs) {
                for (const auto input : inputs) {
                    auto it = commandMap.find(input->getCommandType());
                    if (it != commandMap.end()) {
                        it->second->execute(repository, input);
                    } else {
                        std::cout << "There is no supported command for type: " << input->getCommandType() << std::endl;
                    }
                }
            }

        private:
            std::unordered_map<std::string, Command<TestInput>*> commandMap;
        };
    } // namespace frame_repository_test

} // namespace vision

#endif // VISION_FRAME_REPOSITORY_TEST_COMMAND_INVOKER_H

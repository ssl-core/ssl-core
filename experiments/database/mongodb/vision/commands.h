#ifndef VISION_FRAME_REPOSITORY_TEST_COMMANDS_H
#define VISION_FRAME_REPOSITORY_TEST_COMMANDS_H

#include <iostream>
#include "vision/db/iframe_repository.h"
#include "vision/test_inputs.h"

namespace vision {

    namespace frame_repository_test {
        
        template<class ReturnType>
        class Command {
        public:
            virtual ~Command() {}
            virtual ReturnType execute(IFrameRepository* repository, TestInput* input) = 0;        
        };

        class SaveCommand : public Command<void> {
        public:
            void execute(IFrameRepository* repository, TestInput* input) override {
                auto saveInput = dynamic_cast<SaveInput*>(input);
                if (saveInput) {                    
                    auto func = [=]() {
                        Frame frame = saveInput->frame();
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

        class FindCommand : public Command<Frame> {
        public:
            std::optional<Frame> execute(IFrameRepository* repository, TestInput* input) override {
                auto findInput = dynamic_cast<FindInput*>(input);
                if (findInput) {
                    int id = findInput->id();
                    auto func = [=]() {
                        std::cout << "Finding Frame with ID " << id << std::endl;
                        if (auto frame = repository->find(id)) {
                            std::cout << "Retrieved Frame " << frame.DebugString() << std::endl;
                            return frame;
                        }                
                        std::cout << "Couldn't find a Frame with ID " << id << std::endl;
                        return std::nullopt;
                    };
                    const std::optional<Frame> frame = func();
                    return frame;
                }
                std::cout << "Invalid input for FindCommand." << std::endl;                
            }
        };

        class FindRangeCommand : public Command<std::vector<Frame>> {
        public:
            std::vector<Frame> execute(IFrameRepository* repository, TestInput* input) override {
                auto findRangeInput = dynamic_cast<FindRangeInput*>(input);
                    if (findRangeInput) {
                        int64_t key_lower_bound = findRangeInput->key_lower_bound();
                        int64_t key_upper_bound = findRangeInput->key_lower_bound();
                        auto func = [=]() {
                            std::cout << "Finding Frame where " << key_lower_bound << "=< key <= " << key_upper_bound << std::endl;
                            const std::vector<Frame> frames = repository->find_range(key_lower_bound, key_upper_bound);
                            std::cout << "Retrieved Frames" << std::endl;
                            for(const auto& frame: frames) {
                                std::cout << frame.DebugString() << std::endl;
                            }
                            return frames;
                        }
                        const std::vector<Frame> frames = func();
                        return frames;
                    }                    
                    std::cout << "Invalid input for FindRangeCommand." << std::endl;
                }                 
            }
        };

        class RemoveCommand : public Command<void> {
        public:
            void execute(IFrameRepository* repository, TestInput* input) override {
                auto removeInput = dynamic_cast<RemoveInput*>(input);
                if (removeInput) {
                    auto func = [=]() {
                        int id = removeInput->id();
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

    } // namespace frame_repository_test

} // namespace vision

#endif // VISION_FRAME_REPOSITORY_TEST_COMMANDS_H

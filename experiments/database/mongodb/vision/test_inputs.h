#ifndef VISION_FRAME_REPOSITORY_TEST_TEST_INPUTS_H
#define VISION_FRAME_REPOSITORY_TEST_TEST_INPUTS_H

#include "protocols/vision/frame.pb.h"
#include <string>

using protocols::vision::Frame;

namespace vision {

  namespace frame_repository_test {
  
    class TestInput {
    public:
        virtual ~TestInput() {}
        virtual const std::string& getCommandType() const = 0;
    };

    class SaveInput : public TestInput {
    public:
        explicit SaveInput(Frame frame): frame_(frame) {}
                
        const std::string& getCommandType() const override { return command_type; }
        Frame frame() { return frame_; }

    private:
        const std::string command_type = "save";
        Frame frame_;
    };

    class FindInput : public TestInput {
    public:
        explicit FindInput(int id): id_(id) {}
        
        const std::string& getCommandType() const override { return command_type; }
        int id() { return id_; }
    private:
        const std::string command_type = "find";
        int id_;
    };

    class FindRangeInput : public TestInput {
    public:
        explicit FindRangeInput(int64_t key_lower_bound, int64_t key_upper_bound): key_lower_bound_(key_lower_bound), key_upper_bound_(key_upper_bound) {}

        const std::string& getCommandType() const override { return command_type; }
        int64_t key_lower_bound() { return key_lower_bound_; }
        int64_t key_upper_bound() { return key_upper_bound_; }
    private:
        const std::string command_type = "find_range";
        int64_t key_lower_bound_;
        int64_t key_upper_bound_;
    };

    class RemoveInput : public TestInput {
    public:
        explicit RemoveInput(int id): id_(id) {}

        int id() { return id_; }
        const std::string& getCommandType() const override { return command_type; }
    private:
        const std::string command_type = "remove";
        int id_;
    };

    } // namespace frame_repository_test

} // namespace vision

#endif // VISION_FRAME_REPOSITORY_TEST_TEST_INPUTS_H

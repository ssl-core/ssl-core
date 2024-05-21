#include "commands.h"
#include "vision/command_invoker.h"
#include "vision/db/irepository_factory.h"
#include "vision/db/repository_factory_mapping.h"

#include <iostream>
#include <memory>
#include <vector>

using protocols::vision::Field;
using protocols::vision::Frame;
using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

using vision::frame_repository_test::Command;
using vision::frame_repository_test::CommandInvoker;
using vision::frame_repository_test::FindCommand;
using vision::frame_repository_test::FindInput;
using vision::frame_repository_test::FindRangeCommand;
using vision::frame_repository_test::FindRangeInput;
using vision::frame_repository_test::RemoveCommand;
using vision::frame_repository_test::RemoveInput;
using vision::frame_repository_test::SaveCommand;
using vision::frame_repository_test::SaveInput;
using vision::frame_repository_test::TestInput;

Frame makeFrame(int& serial_id);
void testDatabase();

int main() {
  std::cout << "Creating factory and frame repository." << std::endl;
  const auto kFactory = RepositoryFactoryMapping{}[RepositoryType::MongoDb];

  std::unique_ptr<IFrameRepository> frame_repository = kFactory->createFrameRepository();
  std::future<bool> connection_status = frame_repository->connect();

  if (connection_status.wait(); connection_status.get()) {
    std::cout << "Connected to the database." << std::endl;
  } else {
    std::cout << "Failed to connect to the database." << std::endl;
    return 1;
  }

  std::cout << "Creating commands..." << std::endl;
  std::unique_ptr<SaveCommand> save_command = std::make_unique<SaveCommand>();
  std::unique_ptr<FindCommand> find_command = std::make_unique<FindCommand>();
  std::unique_ptr<FindRangeCommand> find_range_command = std::make_unique<FindRangeCommand>();
  std::unique_ptr<RemoveCommand> remove_command = std::make_unique<RemoveCommand>();

  CommandInvoker invoker;
  invoker.addCommand("save", save_command);
  invoker.addCommand("find", find_command);
  invoker.addCommand("find_range", find_range_command);
  invoker.addCommand("remove", remove_command);

  int serial_id = 0;
  std::cout << "Creating test inputs..." << std::endl;
  const std::vector<TestInput> test_inputs
      = {SaveInput{makeFrame(serial_id)},
         SaveInput{makeFrame(serial_id)},
         FindInput{/*id=*/1},
         FindInput{/*id=*/2},
         SaveInput{makeFrame(serial_id)},
         FindInput{/*id=*/3},
         RemoveInput{/*id=*/3},
         FindInput{/*id=*/3},
         FindRangeInput{/*key_lower_bound=*/1, /*key_upper_bound=*/2}};

  std::cout << "Invoking commands..." << std::endl;
  invoker.executeCommands(frame_repository, test_inputs);

  return 0;
}
//
//
//
//
//
//
//
//
//
//
//
Frame makeFrame(int& serial_id) {
  Frame frame;

  frame.mutable_properties()->set_serial_id(++serial_id);

  Field& field = *frame.mutable_field();
  field.set_length(9000);
  field.set_width(6000);
  field.set_goal_depth(180);
  field.set_goal_width(1000);
  field.set_penalty_area_depth(1000);
  field.set_penalty_area_width(2000);
  field.set_boundary_width(300);
  field.set_goal_center_to_penalty_mark(6000);

  return frame;
}

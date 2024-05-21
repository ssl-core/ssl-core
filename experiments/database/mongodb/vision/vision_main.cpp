#include "vision/db/irepository_factory.h"
#include "vision/db/repository_factory_mapping.h"
#include "vision/command_invoker.h"

#include <iostream>
#include <vector>

using protocols::vision::Field;
using protocols::vision::Frame;
using vision::IFrameRepository;
using vision::RepositoryFactoryMapping;
using vision::RepositoryType;

using vision::frame_repository_test::TestInput;
using vision::frame_repository_test::SaveInput;
using vision::frame_repository_test::FindInSaveInput;
using vision::frame_repository_test::FindRangeInSaveInput;
using vision::frame_repository_test::RemoveInSaveInput;
using vision::frame_repository_test::SaveCommand;
using vision::frame_repository_test::FindCommand;
using vision::frame_repository_test::FindRangeCommand;
using vision::frame_repository_test::RemoveCommand;
using vision::frame_repository_test::CommandInvoker;

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
  SaveCommand saveCommand;
  FindCommand findCommand;
  FindRangeCommand findRangeCommand;
  RemoveCommand removeCommand;

  CommandInvoker invoker;
  invoker.addCommand("save", &saveCommand);
  invoker.addCommand("find", &findCommand);
  invoker.addCommand("find_range", &findRangeCommand);
  invoker.addCommand("remove", &removeCommand);

  int serial_id = 0;
  std::cout << "Creating test inputs..." << std::endl;
  const std::vector<TestInput> test_inputs = {
    SaveInput{makeFrame(serial_id)},
    SaveInput{makeFrame(serial_id)},
    FindInput{1}
    FindInput{2},
    SaveInput{makeFrame(serial_id)},
    FindInput{3},
    RemoveInput{3},
    FindInput{3},
    FindRangeInput{1, 2}
  };

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

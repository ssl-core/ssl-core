#ifndef REFEREE_GAME_EVENT_GAME_EVENT_MAPPER_H
#define REFEREE_GAME_EVENT_GAME_EVENT_MAPPER_H

#include <google/protobuf/arena.h>
#include <protocols/common/game_event.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <robocin/memory/observer_ptr.h>

namespace referee {

class GameEventMapper {
 public:
  GameEventMapper(bool home_is_blue_team, ::robocin::observer_ptr<::google::protobuf::Arena> arena);

  ::robocin::observer_ptr<::protocols::common::GameEvent> fromTimestampAndGameControllerEvent(
      const ::google::protobuf::Timestamp& timestamp,
      const ::protocols::third_party::game_controller::GameEvent& game_event);

 private:
  bool home_is_blue_team_;
  ::robocin::observer_ptr<::google::protobuf::Arena> arena_;
};

} // namespace referee

#endif // REFEREE_GAME_EVENT_GAME_EVENT_MAPPER_H

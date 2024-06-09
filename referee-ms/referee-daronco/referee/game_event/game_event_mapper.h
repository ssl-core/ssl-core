#ifndef REFEREE_GAME_EVENT_GAME_EVENT_MAPPER_H
#define REFEREE_GAME_EVENT_GAME_EVENT_MAPPER_H

#include <absl/container/flat_hash_map.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/repeated_ptr_field.h>
#include <protocols/common/game_event.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>

namespace referee {

class IGameEventMapper {
 public:
  IGameEventMapper() = default;

  IGameEventMapper(const IGameEventMapper&) = delete;
  IGameEventMapper& operator=(const IGameEventMapper&) = delete;
  IGameEventMapper(IGameEventMapper&&) = delete;
  IGameEventMapper& operator=(IGameEventMapper&&) = delete;

  virtual ~IGameEventMapper() = default;

  virtual ::robocin::object_ptr<
      ::google::protobuf::RepeatedPtrField<::protocols::common::GameEvent>>
  gameEventsFromReferee(const ::protocols::third_party::game_controller::Referee& referee);

  virtual ::robocin::object_ptr<
      ::google::protobuf::RepeatedPtrField<::protocols::referee::GameStatus::GameEventsProposal>>
  gameEventsProposalFromReferee(const ::protocols::third_party::game_controller::Referee& referee);
};

class GameEventMapper : public IGameEventMapper {
 public:
  explicit GameEventMapper(::robocin::object_ptr<::google::protobuf::Arena> arena);

  ::robocin::object_ptr<::google::protobuf::RepeatedPtrField<::protocols::common::GameEvent>>
  gameEventsFromReferee(const ::protocols::third_party::game_controller::Referee& referee) override;

  ::robocin::object_ptr<
      ::google::protobuf::RepeatedPtrField<::protocols::referee::GameStatus::GameEventsProposal>>
  gameEventsProposalFromReferee(
      const ::protocols::third_party::game_controller::Referee& referee) override;

 private:
  // serialized third party GameEvent as keys.
  ::absl::flat_hash_map<std::string, ::protocols::common::GameEvent> game_events_map_;
  ::absl::flat_hash_map<std::string, ::protocols::common::GameEvent> game_events_proposal_map_;

  ::robocin::object_ptr<::google::protobuf::Arena> arena_;
};

} // namespace referee

#endif // REFEREE_GAME_EVENT_GAME_EVENT_MAPPER_H

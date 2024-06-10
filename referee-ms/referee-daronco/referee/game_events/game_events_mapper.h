#ifndef REFEREE_GAME_EVENT_GAME_EVENTS_MAPPER_H
#define REFEREE_GAME_EVENT_GAME_EVENTS_MAPPER_H

#include <absl/container/flat_hash_map.h>
#include <google/protobuf/repeated_ptr_field.h>
#include <protocols/common/game_event.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/event.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>

namespace referee {

class IGameEventsMapper {
 public:
  IGameEventsMapper() = default;

  IGameEventsMapper(const IGameEventsMapper&) = delete;
  IGameEventsMapper& operator=(const IGameEventsMapper&) = delete;
  IGameEventsMapper(IGameEventsMapper&&) = default;
  IGameEventsMapper& operator=(IGameEventsMapper&&) = default;

  virtual ~IGameEventsMapper() = default;

  virtual ::google::protobuf::RepeatedPtrField<::protocols::common::GameEvent>
  fromReferee(const ::protocols::third_party::game_controller::Referee& referee) = 0;

  virtual ::google::protobuf::RepeatedPtrField<::protocols::referee::GameStatus::GameEventsProposal>
  proposalsFromReferee(const ::protocols::third_party::game_controller::Referee& referee) = 0;
};

class GameEventsMapper : public IGameEventsMapper {
 public:
  GameEventsMapper() = default;

  ::google::protobuf::RepeatedPtrField<::protocols::common::GameEvent>
  fromReferee(const ::protocols::third_party::game_controller::Referee& referee) override;

  ::google::protobuf::RepeatedPtrField<::protocols::referee::GameStatus::GameEventsProposal>
  proposalsFromReferee(const ::protocols::third_party::game_controller::Referee& referee) override;

 private:
  // serialized third party GameEvent as keys.
  ::absl::flat_hash_map<std::string, ::protocols::common::GameEvent> game_events_map_;
  ::absl::flat_hash_map<std::string, ::protocols::common::GameEvent> game_events_proposal_map_;
};

} // namespace referee

#endif // REFEREE_GAME_EVENT_GAME_EVENTS_MAPPER_H

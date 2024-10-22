#include "communication/messaging/receiver/payload.h"

namespace communication {
namespace {

namespace rc {

using ::protocols::navigation::Navigation;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

} // namespace

Payload::Payload(std::vector<tp::Referee> referee, std::vector<rc::Navigation> navigation) :
    referee_{std::move(referee)},
    navigation_{std::move(navigation)} {}

std::span<const rc::Navigation> Payload::getNavigation() const { return navigation_; }

std::span<const tp::Referee> Payload::getReferee() const { return referee_; }

bool Payload::empty() const { return referee_.empty() and navigation_.empty(); }

} // namespace communication

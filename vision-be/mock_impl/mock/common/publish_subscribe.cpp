#include "mock/common/publish_subscribe.h"

namespace ines {

ITopicSubscriber::ITopicSubscriber(std::string_view topic) : topic_(topic) {}

} // namespace ines

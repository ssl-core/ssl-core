#ifndef VISION_NETWORK_ISENDER_H
#define VISION_NETWORK_ISENDER_H

#include <protocols/vision/frame.pb.h>

namespace vision {

class ISenderCommunication {
 public:
  ISenderCommunication() = default;
  virtual ~ISenderCommunication() = default;
  ISenderCommunication(const ISenderCommunication&) = default;
  ISenderCommunication& operator=(const ISenderCommunication&) = default;
  ISenderCommunication(ISenderCommunication&&) = default;
  ISenderCommunication& operator=(ISenderCommunication&&) = default;

  // Send data to the address.
  // TODO($ISSUE_N): Use template.
  virtual void send(const protocols::vision::Frame& frame) = 0;

  // Binds the address.
  // TODO($ISSUE_N): Add needed bind parameters.
  virtual void bind(std::string_view address, std::string_view topic) = 0;
};

} // namespace vision

#endif // VISION_NETWORK_ISENDER_H

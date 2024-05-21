#ifndef VISION_DB_IFRAME_REPOSITORY_H
#define VISION_DB_IFRAME_REPOSITORY_H

#include "protocols/vision/frame.pb.h"
#include "vision/db/irepository.h"

namespace vision {

using IFrameRepository = IRepository<int64_t, protocols::vision::Frame>;

} // namespace vision

#endif // VISION_DB_IFRAME_REPOSITORY_H

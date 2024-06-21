#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_POINT_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_POINT_H

// TODO(matheusvtna): Use Point from robocin::geometry library instead.
namespace perception {

struct Point2D {
  float x;
  float y;
};

struct Point3D {
  float x;
  float y;
  float z;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_POINT_H

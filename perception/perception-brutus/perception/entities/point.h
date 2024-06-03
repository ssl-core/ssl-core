#ifndef PERCEPTION_ENTITIES_POINT_H
#define PERCEPTION_ENTITIES_POINT_H

// TODO(#ISSUE_N): Use Point from robocin::geometry library instead.
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

#endif // PERCEPTION_ENTITIES_POINT_H

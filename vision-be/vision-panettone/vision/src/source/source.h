// Source.h
#ifndef VISION_SRC_SOURCE_H
#define VISION_SRC_SOURCE_H

#include <iostream>

enum class Source {
  SIMULATION = 1,
  SSLVISION = 2
};

std::ostream& operator<<(std::ostream& os, const Source& source);

#endif // VISION_SRC_SOURCE_H

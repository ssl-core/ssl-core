// Color.h
#ifndef VISION_SRC_COLOR_H
#define VISION_SRC_COLOR_H

#include <iostream>

enum class Color {
  BLUE = 1,
  YELLOW = 2
};

std::ostream& operator<<(std::ostream& os, const Color& color);

#endif // VISION_SRC_COLOR_H

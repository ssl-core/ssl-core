// Color.cpp
#include "vision/src/robot/color/color.h"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const Color& color) {
  switch (color) {
    case Color::BLUE: os << "BLUE"; break;
    case Color::YELLOW: os << "YELLOW"; break;
    default: os << "Unknown Color";
  }
  return os;
}

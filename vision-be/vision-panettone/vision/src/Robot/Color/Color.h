// Color.h
#ifndef COLOR_H
#define COLOR_H

#include <iostream>

enum class Color {
  BLUE = 1,
  YELLOW = 2
};

std::ostream& operator<<(std::ostream& os, const Color& color);
#endif // COLOR_H

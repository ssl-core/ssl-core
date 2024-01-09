// Source.h
#ifndef SOURCE_H
#define SOURCE_H

#include <iostream>

enum class Source {
  SIMULATION = 1,
  SSLVISION = 2
};

std::ostream& operator<<(std::ostream& os, const Source& source);

#endif // SOURCE_H

// Source.cpp
#include "Source.h"

std::ostream& operator<<(std::ostream& os, const Source& source) {
  switch (source) {
    case Source::SIMULATION: os << "SIMULATION"; break;
    case Source::SSLVISION: os << "SSLVISION"; break;
    default: os << "Unknown Source";
  }
  return os;
}

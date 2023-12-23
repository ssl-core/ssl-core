#include <format>
#include <cstdio>

int main() {
  std::puts(std::format("Hello, {}!", "panettone").c_str());
  return 0;
}
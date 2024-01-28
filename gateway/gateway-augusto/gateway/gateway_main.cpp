#include <format>
#include <cstdio>

int main() {
  std::puts(std::format("Hello, {}!", "Sherlock").c_str());
  return 0;
}
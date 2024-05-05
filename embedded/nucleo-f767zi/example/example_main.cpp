#include "example/print.h"
#include "mbed.h" // IWYU pragma: keep

int main() {
  example::println("Hello, {}-{}!", "nucleo", "f767zi");
  return 0;
}

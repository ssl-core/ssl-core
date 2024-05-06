#include "example/print.h"
#include "mbed.h" // IWYU pragma: keep

// mbed-tools detect // show boards
// mbed-tools sterm // open serial terminal

// inside bin directory:
// mbedflash simple -i ${BINARY} --target_id ${TARGET_ID} // flash

int main() {
  robocin::println("Hello, {}-{}!", "nucleo", "f767zi");
  return 0;
}

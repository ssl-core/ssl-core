#include "mbed.h" // IWYU pragma: keep

#include <format>

// https://clangd.llvm.org/troubleshooting#cant-find-standard-library-headers-map-stdioh-etc
// https://os.mbed.com/cookbook/mbed-cmake
// https://github.com/ARMmbed/mbed-os/blob/master/tools/cmake/README.md

// mbed-tools new .
// mbed-tools configure -m nucleo_f767zi -t GCC_ARM

// mbed-tools deploy
// mbed-tools configure -m nucleo_f767zi -t GCC_ARM

// sudo rsync -av --remove-source-files arm-none-eabi/ /usr/local/

int main() {
  std::puts(std::format("Hello, {}!", "Mbed").c_str());

  return 0;
}

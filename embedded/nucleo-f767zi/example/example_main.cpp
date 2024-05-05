#include "mbed.h"

// https://clangd.llvm.org/troubleshooting#cant-find-standard-library-headers-map-stdioh-etc
// https://os.mbed.com/cookbook/mbed-cmake
// https://github.com/ARMmbed/mbed-os/blob/master/tools/cmake/README.md

// mbed-tools new .
// mbed-tools configure -m nucleo_f767zi -t GCC_ARM

// mbed-tools deploy
// mbed-tools configure -m nucleo_f767zi -t GCC_ARM

int main() {
  printf("Hello, Mbed!\n");

  return 0;
}

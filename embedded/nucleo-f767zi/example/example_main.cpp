#include "example/example.nanopb.h"
#include "example/print.h"
#include "mbed.h" // IWYU pragma: keep
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

#include <version>

// mbed-tools detect // show boards
// mbed-tools sterm // open serial terminal

// inside bin directory:
// mbedflash simple -i ${BINARY} --target_id ${TARGET_ID} // flash

// pip install --upgrade google-api-python-client

int main() {
  robocin::println("Hello, {}-{}!", "nucleo", "f767zi");

  example_ExampleNanoPb2 example = {
      .integer = 10,
      .text = "jose",
  };

  std::array<uint8_t, 256> buffer{};
  pb_ostream_t stream = pb_ostream_from_buffer(buffer.data(), sizeof(buffer));
  pb_encode(&stream, example_ExampleNanoPb2_fields, &example);

  std::string bytes(reinterpret_cast<const char*>(buffer.data()), stream.bytes_written);

  return 0;
}

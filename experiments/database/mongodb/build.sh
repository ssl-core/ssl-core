rm -rf build bin
cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++
cmake --build build

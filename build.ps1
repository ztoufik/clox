cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug -G Ninja -S . -B build;
cd build && ninja && ctest;
cd ..

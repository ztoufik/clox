#cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug -G Ninja -S . -B build;
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILER="clang.exe" -DCMAKE_CXX_COMPILER="clang++.exe" -G Ninja -S . -B build;
cd build && ninja && ctest --output-on-failure;
#cd build && ninja && ctest 
cd ..

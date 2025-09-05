$Arg=$args[0]

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILER="clang.exe" -DCMAKE_CXX_COMPILER="clang++.exe" -G Ninja -S . -B build;
cd build 
if ((ninja) -and $Arg) { ctest --output-on-failure -R $Arg }
cd ..

$Arg=$args[0]

#cmake -DLLVM_DIR="D:/LLVM/lib/cmake/llvm" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILER="clang.exe" -DCMAKE_CXX_COMPILER="clang++.exe" -G Ninja -S . -B build;
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILER="clang.exe" -DCMAKE_CXX_COMPILER="clang++.exe" -G Ninja -S . -B build;
cd build && ninja 
switch ($Arg) {
    "main" {.\test\main.exe }
    "LexerText" { ctest --output-on-failure -R $Arg }
    "ParserText" { ctest --output-on-failure -R $Arg  }
}
if ($? -and $Arg) { ctest --output-on-failure -R $Arg }
cd ..

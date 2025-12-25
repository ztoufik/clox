$Arg=$args[0]

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_C_COMPILER="clang.exe" -DCMAKE_CXX_COMPILER="clang++.exe" -G Ninja -S . -B build -DTARGET_TO_BUILD="$Arg";
cd build && ninja 
if($?){
    switch ($Arg) {
        "main" {.\test\main.exe }
        "Lexer" { ctest --output-on-failure -R "LexerTest"}
        "Parser" { ctest --output-on-failure -R "ParserTest"}
    }
}
cd ..

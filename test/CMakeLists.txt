include(FetchContent)
FetchContent_Declare(gtest
    QUIET
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    DOWNLOAD_EXTRACT_TIMESTAMP True
)
# configure build of googletest
include(GoogleTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

set(LLVM_DIR "D:/LLVM/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

set(TARGET_TO_BUILD "main" CACHE STRING "Select which app to build: main, Lexer or Parser")

if(TARGET_TO_BUILD STREQUAL "main")
    add_executable(main main.cpp)
    #target_compile_options(main PRIVATE -w)
    target_include_directories(main PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(main PRIVATE ${PROJECT_NAME} LLVMCore LLVMSupport)
    target_include_directories(main PRIVATE ${LLVM_INCLUDE_DIRS})
elseif(TARGET_TO_BUILD STREQUAL "Lexer")
    add_executable( lexer_unit_tests "lexer_test.cpp")
    target_include_directories(lexer_unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)
    #target_compile_options(lexer_unit_tests PRIVATE -w)
    target_link_libraries(lexer_unit_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)
    gtest_discover_tests(lexer_unit_tests PROPERTIES LABELS "unit" DISCOVERY_TIMEOUT 240)
elseif(TARGET_TO_BUILD STREQUAL "Parser")
    add_executable(parser_unit_tests "parser_test.cpp")
    target_include_directories(parser_unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)
    #target_compile_options(parser_unit_tests PRIVATE -w)
    target_link_libraries(parser_unit_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)
    gtest_discover_tests(parser_unit_tests PROPERTIES LABELS "unit" DISCOVERY_TIMEOUT 240)

else()
    message(FATAL_ERROR "Invalid target: ${TARGET_TO_BUILD}. Choose main , Lexer or Parser.")
endif()

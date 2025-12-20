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

file(GLOB SOURCES *.cpp)

add_executable(main main.cpp)
target_compile_options(main PRIVATE -w)

add_executable( lexer_unit_tests "lexer_test.cpp")
target_compile_options(lexer_unit_tests PRIVATE -w)
target_link_libraries(lexer_unit_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)

# automatic discovery of lexer unit tests
gtest_discover_tests(lexer_unit_tests
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )

add_executable(parser_unit_tests "parser_test.cpp")
target_compile_options(parser_unit_tests PRIVATE -w)
target_link_libraries(parser_unit_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)

# automatic discovery of parser unit tests
gtest_discover_tests(parser_unit_tests
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )


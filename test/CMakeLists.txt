include(FetchContent)
FetchContent_Declare(gtest
  QUIET
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP True
)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

file(GLOB SOURCES *.cpp)

add_executable(
    lexer_unit_tests
    ${SOURCES}
    )

target_link_libraries(lexer_unit_tests
  PRIVATE
    ${PROJECT_NAME}
    GTest::gtest_main
  )

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(lexer_unit_tests
  PROPERTIES
    LABELS "unit"
  DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
    240
  )

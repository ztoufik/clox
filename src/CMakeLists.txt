add_library(${PROJECT_NAME} lexer.cpp parser.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
get_target_property(SRCS tua_core SOURCES)
message(STATUS "tua_core sources: ${SRCS}")

add_library(${PROJECT_NAME} SHARED)

file(GLOB SOURCES *.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})


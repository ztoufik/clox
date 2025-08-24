cmake_minimum_required(VERSION 4.1.0)

project(tua)

# set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

enable_testing()

add_subdirectory(${PROJECT_SOURCE_DIR}/test)

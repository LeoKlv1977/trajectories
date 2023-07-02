cmake_minimum_required(VERSION 3.13)
project(tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(lib ${CMAKE_CURRENT_BINARY_DIR}/lib.make)
add_subdirectory(utests ${CMAKE_CURRENT_BINARY_DIR}/utests.make)

# create executables
add_executable(tracker main.cpp)

target_link_libraries(tracker lib)
target_include_directories(tracker PRIVATE lib)

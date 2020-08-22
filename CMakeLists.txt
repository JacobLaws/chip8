cmake_minimum_required(VERSION 3.15)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GCC_COVERAGE_COMPILE_FLAGS "-static-libgcc -static-libstdc++ -ggdb -o -Wall -g -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp chip8.h chip8.cpp graphics.cpp graphics.h)
add_executable(chip8 ${SOURCE_FILES})

target_link_libraries(chip8 ${SDL2_LIBRARIES})
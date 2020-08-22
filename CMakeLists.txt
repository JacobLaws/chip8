cmake_minimum_required(VERSION 3.15)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GCC_COVERAGE_COMPILE_FLAGS "-static-libgcc -static-libstdc++ -ggdb -o -Wall -g -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(MAIN_SOURCE src/main.cpp)
set(CHIP8_SOURCE src/chip8.hpp src/chip8.cpp)
set(OPCODES_SOURCE src/opcodes.hpp src/opcodes.cpp)
set(PLATFORM_SOURCE src/emulator.hpp src/emulator.cpp)

set(SOURCE_FILES ${MAIN_SOURCE} ${CHIP8_SOURCE} ${OPCODES_SOURCE} ${PLATFORM_SOURCE})
add_executable(chip8 ${SOURCE_FILES})

target_link_libraries(chip8 ${SDL2_LIBRARIES})
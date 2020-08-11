cmake_minimum_required(VERSION 3.15)
project(chip_8)

set(CMAKE_CXX_STANDARD 20)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(chip_8 main.cpp chip8.h chip8.cpp graphics.h graphics.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(chip_8 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

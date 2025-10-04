cmake_minimum_required(VERSION 3.10)
project(imgproc C)

set(CMAKE_C_STANDARD 11)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendors
)

file(GLOB SRC_FILES src/*.c)

add_executable(imgproc ${SRC_FILES})

add_executable(test_runner test/test_runner.c ${SRC_FILES})

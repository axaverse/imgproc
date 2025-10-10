cmake_minimum_required(VERSION 3.10)
project(imgproc C)

set(CMAKE_C_STANDARD 11)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendors
)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.c)
set(MAIN_FILE ${PROJECT_SOURCE_DIR}/src/main.c)
list(REMOVE_ITEM SRC_FILES ${MAIN_FILE})

add_library(imgproc_core ${SRC_FILES})

add_executable(imgproc ${MAIN_FILE})
target_link_libraries(imgproc PRIVATE imgproc_core)

add_executable(test_runner test/test_runner.c)
target_link_libraries(test_runner PRIVATE imgproc_core)
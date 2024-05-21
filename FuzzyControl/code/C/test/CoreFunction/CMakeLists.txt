cmake_minimum_required(VERSION 3.5.0)
project(test_CoreFunction)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/../../lib/FuzzyControl.a)

include_directories(${PROJECT_SOURCE_DIR}/../../src)

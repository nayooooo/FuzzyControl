cmake_minimum_required(VERSION 3.5.0)
project(FuzzyControl VERSION 1.0.0
        DESCRIPTION "This library can be used to calculate the fuzzy output of fuzzy inputs under custom rules"
        HOMEPAGE_URL "https://github.com/nayooooo/ICA.git"
        LANGUAGES "C")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/core/*.c)
set(SRC ${SRC_LIST})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/core)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

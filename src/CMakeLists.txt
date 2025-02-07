cmake_minimum_required(VERSION 3.10.0)
project(FuzzyControl VERSION 1.0.0
        DESCRIPTION "This library can be used to calculate the fuzzy output of fuzzy inputs under custom rules"
        HOMEPAGE_URL "https://github.com/nayooooo/FuzzyControl.git"
        LANGUAGES "C")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_CONFIG_LIST ${PROJECT_SOURCE_DIR}/config/*.c)
file(GLOB SRC_EXTER_LIST ${PROJECT_SOURCE_DIR}/exter/*.c)
file(GLOB SRC_CORE_LIST ${PROJECT_SOURCE_DIR}/core/*.c)
file(GLOB SRC_PERIPH_LIST ${PROJECT_SOURCE_DIR}/periph/*.c)
set(SRC "")
list(APPEND SRC ${SRC_CONFIG_LIST})
list(APPEND SRC ${SRC_EXTER_LIST})
list(APPEND SRC ${SRC_CORE_LIST})
list(APPEND SRC ${SRC_PERIPH_LIST})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/config)
include_directories(${PROJECT_SOURCE_DIR}/exter)
include_directories(${PROJECT_SOURCE_DIR}/core)
include_directories(${PROJECT_SOURCE_DIR}/periph)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
add_library(${PROJECT_NAME}-static STATIC ${SRC})
add_library(${PROJECT_NAME} SHARED ${SRC})
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

file(REMOVE ${PROJECT_SOURCE_DIR}/../lib/lib${PROJECT_NAME}.dll.a)

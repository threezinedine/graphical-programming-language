cmake_minimum_required(VERSION 3.10)
project(CCompiler)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)

file(
    GLOB 
    SOURCE_FILES 
    "src/*.cpp"
    "src/**/*.cpp"
)

file(
    GLOB 
    HEADER_FILES
    "include/*.h"
    "include/**/*.h"
)

file(
    GLOB_RECURSE
    TEST_FILES
    "src/__tests__/test_*.cpp"
    "src/**/__tests__/test_*.cpp"
)

foreach(TEST_FILE ${TEST_FILES})
    list(REMOVE_ITEM SOURCE_FILES ${TEST_FILE})
endforeach()

set(CMAKE_FOLDER "Libraries")

add_library(
    ${PROJECT_NAME} 
    STATIC 
    ${SOURCE_FILES} ${HEADER_FILES}
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    include
)

target_precompile_headers(
    ${PROJECT_NAME} 
    PUBLIC 
    include/pch.h
)

unset(CMAKE_FOLDER)

set(CMAKE_FOLDER "Dependencies")
find_package(ntt-gtest REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
unset(CMAKE_FOLDER)

set(CMAKE_FOLDER "Tests")

set(TEST_NAME ${PROJECT_NAME}_tests)

add_executable(
    ${TEST_NAME}
    ${TEST_FILES}
    ${CMAKE_SOURCE_DIR}/test.cpp
)

target_link_libraries(
    ${TEST_NAME}
    PUBLIC
    ntt-gtest
    ${PROJECT_NAME}
)

target_include_directories(
    ${TEST_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


unset(CMAKE_FOLDER)

set_property(GLOBAL PROPERTY CMAKE_VS_STARTUP_PROJECT ${PROJECT_NAME})
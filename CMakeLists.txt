cmake_minimum_required(VERSION 3.10)
project(ADVlib)

set(CMAKE_CXX_STANDARD 14)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors/Catch2)

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
file(GLOB_RECURSE LIB_SOURCES "lib/*.h")
set(SOURCE_FILES main.cpp ${LIB_SOURCES} ${TEST_SOURCES})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

include_directories(${HEADER_DIR})
add_executable(ADVlib ${SOURCE_FILES})
target_link_libraries(ADVlib Catch)


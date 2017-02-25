cmake_minimum_required(VERSION 3.6)
project(JSON_interpreter)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp JSON.h JSON.cpp)
add_executable(JSON_interpreter ${SOURCE_FILES})
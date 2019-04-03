cmake_minimum_required(VERSION 3.5)
project(untitled)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl -ljson")
set(CMAKE_CXX_STANDARD 11)
add_executable(untitled main.cpp)
target_link_libraries(untitled curl)
target_link_libraries(untitled json)

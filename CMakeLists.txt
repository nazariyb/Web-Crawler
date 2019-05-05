cmake_minimum_required(VERSION 3.10)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

add_executable(parser
        src/main.cpp src/domain_parser.cpp src/domain_parser.h)
find_package(Threads REQUIRED)
target_link_libraries(parser curl)
target_link_libraries(parser ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(WebCrawler json)


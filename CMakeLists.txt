cmake_minimum_required(VERSION 3.10)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

add_executable(parser
        src/main.cpp src/domain_parser.cpp src/domain_parser.h
        dependencies/TF_IDF.cpp dependencies/TF_IDF.h)

find_package (Eigen3 3.3 REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED system)
find_package(Boost COMPONENTS system filesystem locale REQUIRED)
target_link_libraries(parser curl)
target_link_libraries(parser
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_LOCALE_LIBRARY})
target_link_libraries(parser ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (parser Eigen3::Eigen)
#target_link_libraries(WebCrawler json)


cmake_minimum_required(VERSION 3.10)
project(WebCrawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

set(PARSER parser)
set(SEARCHER searcher)

include_directories(dependencies)

find_package(Qt5Core)
find_package(Qt5Concurrent)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system filesystem locale REQUIRED)

add_executable(${PARSER}
        src/parser/parser.cpp src/parser/domain_parser.cpp src/parser/domain_parser.h
        src/searcher/searcher.cpp)

target_link_libraries(${PARSER}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_LOCALE_LIBRARY}
        curl
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_executable(${SEARCHER}
        src/searcher/searcher.cpp
        dependencies/TF_IDF.cpp dependencies/TF_IDF.h
        dependencies/reader/reader.cpp dependencies/reader/word_usings.h
        dependencies/reader/conf_reader.cpp dependencies/reader/conf_reader.h
        dependencies/reader/file_reader.cpp dependencies/reader/file_reader.h
        dependencies/reader/reader.cpp dependencies/reader/reader.h
        dependencies/concurrancy_helpers/map_manipulation.cpp dependencies/concurrancy_helpers/map_manipulation.h
        dependencies/reader/file_writer.cpp dependencies/reader/file_writer.h)

target_link_libraries(${SEARCHER}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_LOCALE_LIBRARY}
        Qt5::Core
        Qt5::Concurrent
        curl
        ${CMAKE_THREAD_LIBS_INIT}
        )

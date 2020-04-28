cmake_minimum_required(VERSION 3.10)
project(hw_03)

set(CMAKE_CXX_STANDARD 17)

#include(ExternalProject)
#find_package(Git REQUIRED)
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

include_directories(include)
file(GLOB sources include/*.h src/*.cpp)

add_executable(hw_03 ${sources})
# doctest.h was in test/ when i used it
add_executable(test_hw_03
        test/TestMain.cpp
        test/HuffmanTreeTest.cpp
        test/HuffmanArchiverTest.cpp
        test/TestCLI.cpp
        src/HuffmanArchiver.cpp
        src/Huffman.cpp
        src/CLI.cpp
        )
target_include_directories(test_hw_03 PUBLIC ${DOCTEST_INCLUDE_DIR})

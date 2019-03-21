cmake_minimum_required(VERSION 3.5)
project(BFS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

FILE(GLOB SOURCE_FILES *.cpp)

add_executable(run ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.5)
project(BFS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAAKE_CURRENT_SOURCE_DIR}/)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB HEADER_FILES *.h)

add_executable(run ${HEADER_FILES} ${SOURCE_FILES})
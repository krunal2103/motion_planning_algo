cmake_minimum_required(VERSION 3.5)
project(Motion_Planning)

set(CMAKE_CXX_STANDARD 17)

# BFS
add_executable(bfs src/bfs.cpp)

# Dijkstra
add_executable(dijkstra src/dijkstra.cpp)
cmake_minimum_required(VERSION 3.5)
project(Motion_Planning)

# BFS
add_executable(bfs src/bfs.cpp grid/square_grid_interface.cpp)
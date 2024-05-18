cmake_minimum_required(VERSION 3.28)
project(Dijkstra_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Dijkstra_Project
        Dijkstra.cpp
        Dijkstra.h)

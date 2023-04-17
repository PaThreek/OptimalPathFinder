cmake_minimum_required(VERSION 3.22.1)
project(OptimalPath)

set(CMAKE_CXX_STANDARD 20)

add_executable(OptimalPath main.cpp MapManager.h MapManager.cpp Server.h Server.cpp)
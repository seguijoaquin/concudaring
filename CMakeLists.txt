cmake_minimum_required(VERSION 3.5)
project(Concudaring)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/concudaring.cpp src/concudaring.h src/table.cpp src/table.h src/player.cpp src/player.h)
add_executable(Concudaring ${SOURCE_FILES})
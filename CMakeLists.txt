cmake_minimum_required(VERSION 3.5)
project(Concudaring)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")
set(SOURCE_FILES src/main.cpp src/concudaring.cpp src/concudaring.h src/table.cpp src/table.h src/player.cpp src/player.h playground/Semaforo.cpp playground/Semaforo.h)
add_executable(Concudaring ${SOURCE_FILES})

add_executable(Test Test/main.cpp)
target_link_libraries(Test ${CMAKE_SOURCE_DIR}/gtest/libgtest.a ${CMAKE_SOURCE_DIR}/gtest/libgtest_main.a)

set(PLAYGROUND_FILES playground/main.cpp playground/Semaforo.cpp playground/Semaforo.h)
add_executable(Playground ${PLAYGROUND_FILES})
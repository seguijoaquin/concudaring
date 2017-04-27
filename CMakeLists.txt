cmake_minimum_required(VERSION 3.5)
project(Concudaring)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

set(SOURCE_FILES src/main.cpp src/utils/SharedMemory.h src/utils/logger/logger.h src/utils/logger/logger.cpp
        src/utils/logger/mensajes.h src/utils/types.h src/utils/constants.h src/concudaring.cpp src/concudaring.h
        src/table.cpp src/table.h src/player.cpp src/player.h src/utils/Semaforo.cpp src/utils/Semaforo.h
        src/deckOfCards.cpp src/deckOfCards.h src/creatorDeckOfCards.cpp src/creatorDeckOfCards.h
        src/CommunicationChannel.cpp src/CommunicationChannel.h
        src/utils/FifoEscritura.cpp src/utils/FifoEscritura.h src/utils/FifoLectura.cpp
        src/utils/FifoLectura.h src/utils/Fifo.h src/utils/Fifo.cpp)
add_executable(Concudaring ${SOURCE_FILES})

#add_executable(Test Test/main.cpp)
#target_link_libraries(Test ${CMAKE_SOURCE_DIR}/gtest/libgtest.a ${CMAKE_SOURCE_DIR}/gtest/libgtest_main.a)


set(PLAYGROUND_FILES playground/main.cpp playground/Semaforo.cpp playground/Semaforo.h playground/CommunicationChannel.h playground/CommunicationChannel.cpp playground/Fifo.h playground/FifoEscritura.cpp playground/FifoEscritura.h playground/FifoLectura.cpp playground/FifoLectura.h playground/Fifo.cpp)
add_executable(Playground ${PLAYGROUND_FILES})

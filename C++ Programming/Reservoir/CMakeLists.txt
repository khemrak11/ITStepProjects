cmake_minimum_required(VERSION 3.0)
project("Reservoir")
set(INC Reservoir.h LinkedList.h)
set(SRC Reservoir.cpp LinkedList.cpp)
set(SOURCE main.cpp ${INC} ${SRC})
add_executable(${PROJECT_NAME} ${SOURCE})
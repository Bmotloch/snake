cmake_minimum_required(VERSION 3.0.0)
project(Snake VERSION 0.1.0)

add_executable(Snake main.cpp Snake.cpp SnakeTextBoard.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

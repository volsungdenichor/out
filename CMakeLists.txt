cmake_minimum_required(VERSION 3.5)
project(out)

enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

add_subdirectory(src)


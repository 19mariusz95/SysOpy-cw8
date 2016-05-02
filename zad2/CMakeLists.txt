cmake_minimum_required(VERSION 3.5)
project(zad2 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCE_FILES main.c)
add_executable(zad2 ${SOURCE_FILES})
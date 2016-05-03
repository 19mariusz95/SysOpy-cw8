cmake_minimum_required(VERSION 3.4)
project(zad1 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pthread")

add_executable(zad1a zad1a.c main.h)
add_executable(zad1b zad1b.c main.h)
add_executable(zad1c zad1c.c main.h)
add_executable(mkf mkf.c)
cmake_minimum_required(VERSION 3.21)
project(HW1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(HW1
        os.c
        os.h
        pt.c)

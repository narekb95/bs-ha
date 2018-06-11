cmake_minimum_required(VERSION 3.10)
project(bs4 C)

set(CMAKE_C_STANDARD 11)

add_executable(bs4
        main.c
        array.c
        array.h
        ult.c
        ult.h
        )
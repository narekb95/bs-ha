cmake_minimum_required(VERSION 3.10)
project(narek C)

set(CMAKE_C_STANDARD 99)

add_executable(narek main.c strukturen.c strukturen.h ult.c ult.h)
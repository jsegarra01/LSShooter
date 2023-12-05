cmake_minimum_required(VERSION 3.20)
project(shooter C)

set(CMAKE_C_STANDARD 99)

add_executable(shooter main.c Fat16Utilities.c Fat16Utilities.h Ext2Utilities.c Ext2Utilities.h Logic.c Logic.h)

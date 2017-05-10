cmake_minimum_required(VERSION 3.6)
project(shell_simulator)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(shell_simulator ${SOURCE_FILES})
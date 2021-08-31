cmake_minimum_required(VERSION 3.20)
project(bf_machine)

set(CMAKE_CXX_STANDARD 17)

add_executable(bf_machine brainfuck/bfmake.cpp brainfuck/bfmachine.cpp)

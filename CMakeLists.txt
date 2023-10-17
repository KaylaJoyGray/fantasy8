cmake_minimum_required(VERSION 3.27)
project(fantasy8)

set(CMAKE_CXX_STANDARD 20)

add_executable(fantasy8 main.cpp
        chip8_context.hpp
        chip8_context.cpp
        chip8_display.hpp
        target_defines.hpp
        chip8_display.cpp
)

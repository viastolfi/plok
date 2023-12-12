cmake_minimum_required(VERSION 3.20)

project(plok)

set(CMAKE_CXX_STANDARD 20)

add_executable(plok src/main.cpp
        src/tokenize.cpp
        src/parse.hpp
        src/parse.cpp
        src/generation.hpp
        src/generation.cpp
)
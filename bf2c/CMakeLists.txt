cmake_minimum_required (VERSION 2.8)
project(bf2c)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++14 -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

file(
    GLOB_RECURSE
    src_files
    src/*
)

add_executable(bf2c ${src_files})

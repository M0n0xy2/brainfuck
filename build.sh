#!/bin/sh
# llvm-config --cppflags --ldflags --libs core jit native all
clang++ ./src/*.cpp `llvm-config --cppflags --ldflags --libs core jit native all` -pthread -ltinfo -ldl -lz -o ./bin/bf_compiler -std=c++14 -Wall -Wextra -pedantic

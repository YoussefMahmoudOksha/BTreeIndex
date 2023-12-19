cmake_minimum_required(VERSION 3.27)
project(BTreeIndex)

set(CMAKE_CXX_STANDARD 17)

add_executable(BTreeIndex main.cpp
        BTreeIndex.h
        BTreeIndex.cpp)

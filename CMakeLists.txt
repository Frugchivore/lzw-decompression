cmake_minimum_required(VERSION 3.7)
project(LZWDecoder)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp LZWDecoder.cpp LZWDecoder.h)
add_executable(LZWDecoder ${SOURCE_FILES})
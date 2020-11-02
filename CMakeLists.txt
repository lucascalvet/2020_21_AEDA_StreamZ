cmake_minimum_required(VERSION 3.17)
project(StreamZ_AEDA_2020_21)

set(CMAKE_CXX_STANDARD 14)

add_executable(StreamZ_AEDA_2020_21 main.cpp streamz.cpp user.cpp stream.cpp utils.cpp streamz.h user.h stream.h utils.h)
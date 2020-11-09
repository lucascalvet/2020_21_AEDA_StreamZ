cmake_minimum_required(VERSION 3.17)
project(StreamZ_AEDA_2020_21)

set(CMAKE_CXX_STANDARD 14)

add_executable(StreamZ_AEDA_2020_21 main.cpp streamz.cpp streamz.h user.cpp user.h stream.cpp stream.h utils.cpp utils.h menu.cpp menu.h streamz_framework.cpp exceptions.h streamz_framework.h)
cmake_minimum_required(VERSION 3.17)
project(StreamZ_AEDA_2020_21)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(StreamZ_AEDA_2020_21 main.cpp src/streamz.cpp src/streamz.h src/user.cpp src/user.h src/stream.cpp src/stream.h src/utils.cpp src/utils.h
        src/menu.cpp src/menu.h src/streamz_framework.cpp src/exceptions.h src/streamz_framework.h src/tests.cpp)

target_link_libraries(StreamZ_AEDA_2020_21 gtest gtest_main)
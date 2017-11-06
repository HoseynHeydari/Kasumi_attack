cmake_minimum_required(VERSION 3.8)
project(Kasumi_attack)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES kasumi.cpp)
add_executable(Kasumi_attack ${SOURCE_FILES})
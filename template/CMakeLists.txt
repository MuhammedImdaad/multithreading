cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(template VERSION 0.1.0 LANGUAGES C CXX)
add_executable(template main.cpp)
include_directories(.)
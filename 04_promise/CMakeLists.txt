cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(promise VERSION 0.1.0 LANGUAGES C CXX)
add_executable(promise main.cpp)
include_directories(.)
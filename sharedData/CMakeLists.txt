cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sharedData VERSION 0.1.0 LANGUAGES C CXX)
add_executable(sharedData main.cpp)
include_directories(.)
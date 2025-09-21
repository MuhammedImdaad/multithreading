cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(parallel VERSION 0.1.0 LANGUAGES C CXX)

# Standard multithreading
add_executable(parallel main.cpp)
include_directories(.)

# C++17 parallel algorithms
add_executable(parallel_stl parallel_stl.cpp)
find_package(TBB)
if(TBB_FOUND)
    target_link_libraries(parallel_stl TBB::tbb)
endif()
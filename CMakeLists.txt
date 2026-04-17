cmake_minimum_required(VERSION 3.10.0)
project(k-means_algorithm VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(k-means_algorithm K-means_algorithm.cpp)


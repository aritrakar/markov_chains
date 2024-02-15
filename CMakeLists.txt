cmake_minimum_required(VERSION 3.10)  # Specify the minimum version for CMake

project(markov_chains VERSION 1.0)  # Project name and version

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Eigen library includes
include_directories(${CMAKE_SOURCE_DIR}/lib/)

# Add source files
add_executable(mc src/main.cpp src/markov.cpp)

cmake_minimum_required(VERSION 3.10)  # Specify the minimum version for CMake

project(markov_chains VERSION 1.0)  # Project name and version

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Eigen library includes
include_directories(${CMAKE_SOURCE_DIR}/lib/)
include_directories(${CMAKE_SOURCE_DIR}/src/)

# Add source files
add_executable(mc src/main.cpp src/markov.cpp)

# For Googletest

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Test binary name
set(TEST_BINARY_NAME markov_chains_test)

# Add src directory to the include paths
# include_directories("${PROJECT_SOURCE_DIR}/src")

file(GLOB TEST_SOURCES "tests/*.cpp")
file(GLOB SRC "src/*.cpp")
add_executable(${TEST_BINARY_NAME} ${TEST_SOURCES} src/markov.cpp)

target_link_libraries(
    ${TEST_BINARY_NAME}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TEST_BINARY_NAME})

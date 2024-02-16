# Markov Chain Implementation

This repository contains the code for a **finite-state discrete time Markov chain** implementation in **C++** using the **Eigen** library. It's a work in progress and serves to complement my learnings in my stochastic processes class using concepts from graph theory. It also serves as a practice project for using CMake and Googletest.

Note that the files for Eigen were not uploaded.

**Current capabilities:**

1. Simulate $n$ steps of the Markov chain.
1. Find communication classes.
1. Find the steady state.
1. Find the periodicity of states.

**To do**

1. Transcience and recurrence
1. Mean recurrence time

## Prerequisites

1. C++17
1. Make sure you've installed Eigen in a directory called `lib` in this project's directory (see [here](https://eigen.tuxfamily.org/dox/GettingStarted.html))
1. CMake (optional, but recommended)

## How to run the code?

### Using CMake

```bash
mkdir build
cd build

# Configure CMake
cmake ../

# Generate build files
cmake --build .

# Execute
# By Default, the executable is generated under `build/Debug`
./Debug/mc.exe ../etc/tmp1.txt
```

### Using g++

Note that the following will work directly on Linux or MacOS. However, for running on Windows, you'll probably need to use Powershell.

```bash
# Compile and generate executable
# Format: g++ -O3 -o <executable_name> src/*.cpp -I <path to Eigen library>
g++ -O3 -o mc src/*.cpp -I./lib

# Run executable
./mc ./etc/tpm1.txt
```

### Testing

This project uses Googletest for testing. Assuming you have built everything correctly, to execute tests you need only do the following: `cd build && ctest`.

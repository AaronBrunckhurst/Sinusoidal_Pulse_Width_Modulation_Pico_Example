#!/bin/bash

echo "Pulling submodules"
git submodule update --init --recursive

echo "Setting up Pico SDK"
bash PicoSDKSetup/get_pico_sdk.sh

echo "Creating build directory and building"
mkdir -p build
cd build

# Pass the variables to CMake as definitions
cmake ..
make -j4

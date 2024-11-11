#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/home/ubuntu/projects/cpp_libs/helper_libs ..
make
cmake --install .
